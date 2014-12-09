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

#include <Carna/dicom/SeriesLoadingWorker.h>
#include <QStringList>
#include <dicom-interface/dicomImage.h>
#include <QApplication>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// SeriesLoadingWorker
// ----------------------------------------------------------------------------------

SeriesLoadingWorker::SeriesLoadingWorker( const QStringList& filenames
                                        , unsigned int firstIndex
                                        , unsigned int numFiles )
   : filenames( filenames )
   , firstIndex( firstIndex )
   , numFiles( numFiles )
{
}


void SeriesLoadingWorker::run()
{
    canceled = false;

    for( unsigned int i = firstIndex; i < firstIndex + numFiles; ++i )
    {
        if( isCanceled() )
        {
            break;
        }
        const QString filename = filenames.at( i ); 
        DicomImage* dicomImage = new DicomImage();
        if( dicomImage->open( filename.toStdString() ) )
        {
            images.push_back( dicomImage );
        }
        else
        {
            delete dicomImage;
        }
        emit progressed( 1 );
    }

    emit finished();
}



}  // namespace Carna :: dicom

}  // namespace Carna
