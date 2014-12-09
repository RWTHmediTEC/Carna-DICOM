/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/dicom/DicomSceneFactory.h>
#include <Carna/dicom/UInt16VolumeComputationWorker.h>
#include <Carna/dicom/SeriesLoadingWorker.h>
#include <Carna/dicom/SeriesLoadingRequest.h>
#include <Carna/dicom/SeriesElement.h>
#include <Carna/dicom/Series.h>
#include <Carna/dicom/DicomOpeningDialog.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/qt/CarnaProgressDialog.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Parallelization.h>
#include <dicom-interface/dicomImage.h>
#include <dicom-interface/dicomSeries.h>
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <limits>
#include <sstream>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// DicomSceneFactory
// ----------------------------------------------------------------------------------

DicomSceneFactory::DicomSceneFactory
        ( QWidget* dialogParent
        , const base::ParallelizationSettings& parallelizationSettings
        , QObject* parent )
    : base::model::SceneFactory( dialogParent, parallelizationSettings, parent )
{
}


base::model::Scene* DicomSceneFactory::createFromUserInput()
{
    DicomOpeningDialog openingDialog( dialogParent );

    if( openingDialog.exec() == QDialog::Accepted )
    {
        return createFromRequest( openingDialog.getRequest() );
    }
    else
    {
        return nullptr;
    }
}


base::model::Scene* DicomSceneFactory::createFromRequest( const SeriesLoadingRequest& request )
{
    using namespace Carna::base::model;
    DicomSeries ds;

    ds.setSpacingZ( 0 );

    QStringList sFilenameList;
    for( auto element_itr  = request.getSeries().getElements().begin();
              element_itr != request.getSeries().getElements().end();
            ++element_itr )
    {
        const SeriesElement& element = **element_itr;
        sFilenameList.push_back( QString::fromStdString( element.fileName ) );
    }
    
 // -----------------------------------------------------------------

 // create worker threads

    const unsigned int numWorkers = std::min( unsigned( sFilenameList.size() ), unsigned( parallelizationSettings.threads ) );
    const unsigned int filesPerWorker = sFilenameList.size() / numWorkers;
    base::Parallelization< SeriesLoadingWorker > parallel_job;
    for( unsigned int i = 0; i < numWorkers; ++i )
    {
        const unsigned int first = i * filesPerWorker;
        const unsigned int count = ( i + 1 == numWorkers ? sFilenameList.size() - first : filesPerWorker );
        parallel_job.putWorker( new SeriesLoadingWorker( sFilenameList, first, count ) );
    }

 // run workers

    if( !parallel_job.run( "Opening...", parallelizationSettings.priority, dialogParent ) )
    {
        return nullptr; // canceled by user
    }

 // fetch results

    for( unsigned int i = 0; i < numWorkers; ++i )
    {
        SeriesLoadingWorker& worker = parallel_job.workerByIndex( i );

        const SeriesLoadingWorker::DicomImageVector& images = worker.getResult();
        for( auto it = images.begin(); it != images.end(); ++it )
        {
            DicomImage* const dicomImage = *it;

            if( ds.size() == 0 )
            {
                ds.setWidth( dicomImage->getWidth() );
                ds.setHeight( dicomImage->getHeight() );
                ds.setSpacingXY( dicomImage->getSpacingXY() );
            }
            else
            if( ds.size() == 1 )
            {
                ds.setSpacingZ( dicomImage->getPositionZ() - ds.getImage( 0 )->getPositionZ() );
                if( ds.spacingZ() == 0 )
                {
                    QMessageBox::warning( dialogParent, "DICOM Data", "Z-Position of DICOM images is not set. This might cause serious problems with most views." );
                    ds.setSpacingZ( 1.f );
                }
            }
            if( ds.width() != dicomImage->getWidth()
             || ds.height() != dicomImage->getHeight() )
            {
                ds.deleteAll();

                throw std::runtime_error( "Inconsistent image size." );
            }
            ds.addImage( dicomImage );
        }
    }

 // make spacing positive

    ds.setSpacingXY( std::abs( ds.spacingXY() ) );
    ds.setSpacingZ ( request.getSpacingZ() );
    
 // -----------------------------------------------------------------

 // compute required volume size

    const base::Vector3ui full_size( ds.width(), ds.height(), ds.size() );
    const unsigned int longest_side = std::max( full_size.x, std::max( full_size.y, full_size.z ) );

    base::Vector3ui used_size = full_size;
    const auto max_allowed_side_length = maximumAllowedSideLength();
    if( longest_side > max_allowed_side_length )
    {
        used_size.x = ( used_size.x * max_allowed_side_length ) / longest_side;
        used_size.y = ( used_size.y * max_allowed_side_length ) / longest_side;
        used_size.z = ( used_size.z * max_allowed_side_length ) / longest_side;

        const double axis_scaling_factor = static_cast< double >( longest_side ) / max_allowed_side_length;

        ds.setSpacingXY( ds.spacingXY() * axis_scaling_factor );
        ds.setSpacingZ( ds.spacingZ() * axis_scaling_factor );
    }

 // resize if necessary and create scene object

    Scene* const model = createFromDicomSeries( ds, used_size );
    
    ds.deleteAll();

    return model;
}


base::model::Scene* DicomSceneFactory::createFromDicomSeries
    ( const DicomSeries& dicomSeries
    , const base::Vector3ui& size )
{
    using namespace Carna::base::model;
    UInt16Volume::BufferType& data = *( new UInt16Volume::BufferType( size.x * size.y * size.z ) );

    DicomSeries& ds = const_cast< DicomSeries& >( dicomSeries );
    const base::Vector3ui src_size( ds.width(), ds.height(), ds.images()->size() );

    CARNA_ASSERT_EX
        ( size.x >= 2 && size.y >= 2 && size.z >= 2
        , "DICOM data set with less than 2 slices or pixels per image axis not supported" );

    CARNA_ASSERT_EX
        ( size.x <= src_size.x && size.y <= src_size.y && size.z <= src_size.z
        , "destination image can't be larger than source data" );

    const bool down_sampling = size.x < src_size.x
                            || size.y < src_size.y 
                            || size.z < src_size.z;

    const double resolution_reduction = 1 - ( 1. * size.x * size.y * size.z ) / ( src_size.x * src_size.y * src_size.z );

 // parallel execution

    const unsigned int num_workers = getWorkerCount();
    base::Parallelization< UInt16VolumeComputationWorker > parallel_job;

    for( unsigned int i = 0; i < num_workers; ++i )
    {
        parallel_job.putWorker( new UInt16VolumeComputationWorker( ds, size, data ) );
    }

    configureWorkers( parallel_job, size );

    std::stringstream ss_hint_resample;
    ss_hint_resample << "Resampling volume...\nReducing resolution by " << ( 100 * resolution_reduction ) << " %";
    const std::string hint = down_sampling ? ss_hint_resample.str() : "Sampling volume...";
    parallel_job.run( hint, parallelizationSettings.priority, dialogParent );
    
 // -----------------------------------------------------------------

 // create volume

    UInt16Volume* const volume = new UInt16Volume( size, new base::Composition< UInt16Volume::BufferType >( &data ) );

 // create model

    Scene* const model = new Scene( new base::Composition< Volume >( volume )
                                  , ds.spacingXY()
                                  , ds.spacingXY()
                                  , ds.spacingZ () );
    
 // -----------------------------------------------------------------

 // sum cube faces' intensity

    model->setRecommendedVoidThreshold( computeVoidThreshold( model->volume() ) );
    
 // -----------------------------------------------------------------

 // fetch recommended windowing settings

    model->setRecommendedWindowingLevel( ds.getImage( 0 )->getRecommendedWindowLevel() );
    model->setRecommendedWindowingWidth( ds.getImage( 0 )->getRecommendedWindowWidth() );

 // finish

    return model;
}



}  // namespace Carna :: dicom

}  // namespace Carna
