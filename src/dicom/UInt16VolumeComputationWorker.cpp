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

#include <Carna/dicom/UInt16VolumeComputationWorker.h>
#include <Carna/base/Transformation.h>
#include <dicom-interface/dicomImage.h>
#include <dicom-interface/dicomSeries.h>
#include <QApplication>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// series_huv_at
// ----------------------------------------------------------------------------------

signed int series_huv_at
    ( const DicomSeries& dicomSeries
    , unsigned int x
    , unsigned int y
    , unsigned int z )
{
    DicomSeries& ds = const_cast< DicomSeries& >( dicomSeries );
    DicomImage& slice = *( ds.getImage( z ) );
    return slice.getPixel( ds.width() - 1 - x, ds.height() - 1 - y );
}



// ----------------------------------------------------------------------------------
// UInt16VolumeComputationWorker
// ----------------------------------------------------------------------------------

UInt16VolumeComputationWorker::UInt16VolumeComputationWorker( const DicomSeries& dicomSeries
                                                            , const Vector3ui& size
                                                            , DestinationBuffer& dst )
    : UInt16VolumeBaseWorker( dst )
    , dicomSeries( dicomSeries )
    , size( size )  // full target size
{
}


void UInt16VolumeComputationWorker::run()
{
    DicomSeries& ds = const_cast< DicomSeries& >( dicomSeries );

    const Vector3ui src_size( ds.width(), ds.height(), ds.images()->size() );

    unsigned int progress = 0;

    if( src_size == size )
    {

     // simple copy

        const unsigned int progressEmitationThreshold = getSetup().lastVoxel.z / 25; //< ugly patch \o/

        for( unsigned int z = getSetup().firstVoxel.z; z <= getSetup().lastVoxel.z; ++z )
        {
            if( this->isCanceled() )
            {
                emit finished();

                return;
            }

            for( unsigned int x = getSetup().firstVoxel.x; x <= getSetup().lastVoxel.x; ++x )
            for( unsigned int y = getSetup().firstVoxel.y; y <= getSetup().lastVoxel.y; ++y )
            {
                const unsigned int position = array_element_index( size, x, y, z );
                const signed int i0 = series_huv_at( ds, x, y, z );
                dst[ position ] = static_cast< UInt16Volume::VoxelType >( ( i0 + 1024 ) << 4 );
            }

            ++progress;
            if( progress >= progressEmitationThreshold )
            {
                emit progressed( progress );
                progress = 0;
            }

            QApplication::processEvents();
        }

    }
    else
    {

     // down-sampling

        const double src_pixels_per_dst_pixel = src_size.x / static_cast< double >( size.x );
        CARNA_ASSERT( src_pixels_per_dst_pixel >= 1 );
        qDebug( "Average source pixels per destination pixel: %f", src_pixels_per_dst_pixel );

        /** Ugly patch against QProgressDialog stack overflow:  \o/
          */
        const unsigned int progressEmitationThreshold = getSetup().lastVoxel[ 2 ] / 25;

        for( unsigned int z = getSetup().firstVoxel[ 2 ]; z <= getSetup().lastVoxel[ 2 ]; ++z )
        {
            if( this->isCanceled() )
            {
                emit finished();
                return;
            }

         // iterate over all destination pixels

            const unsigned int src_z0 = static_cast< unsigned int >( z * src_pixels_per_dst_pixel );
            const unsigned int src_z1 = static_cast< unsigned int >( ( z + 1 ) * src_pixels_per_dst_pixel );
            CARNA_ASSERT( src_z1 <= src_size.z );
            for( unsigned int y = getSetup().firstVoxel[ 1 ]; y <= getSetup().lastVoxel[ 1 ]; ++y )
            {
                const unsigned int src_y0 = static_cast< unsigned int >( y * src_pixels_per_dst_pixel );
                const unsigned int src_y1 = static_cast< unsigned int >( ( y + 1 ) * src_pixels_per_dst_pixel );
                CARNA_ASSERT( src_y1 <= src_size.y );
                for( unsigned int x = getSetup().firstVoxel[ 0 ]; x <= getSetup().lastVoxel[ 0 ]; ++x )
                {
                    const unsigned int src_x0 = static_cast< unsigned int >( x * src_pixels_per_dst_pixel );
                    const unsigned int src_x1 = static_cast< unsigned int >( ( x + 1 ) * src_pixels_per_dst_pixel );
                    CARNA_ASSERT( src_x1 <= src_size.x );

                 // iterate over all source pixels

                    signed long sum = 0;
                    const std::size_t source_pixels_count = ( src_x1 - src_x0 ) * ( src_y1 - src_y0 ) * ( src_z1 - src_z0 );
                    for( unsigned int src_z = src_z0; src_z < src_z1; ++src_z )
                    for( unsigned int src_y = src_y0; src_y < src_y1; ++src_y )
                    for( unsigned int src_x = src_x0; src_x < src_x1; ++src_x )
                    {
                        const signed int value = series_huv_at( ds, src_x, src_y, src_z );
                        sum += value;
                    }

                 // compute average of source pixels and write result

                    const signed int average_value = sum / static_cast< signed int >( source_pixels_count );
                    const unsigned int destination_index = array_element_index( size, x, y, z );
                    dst.at( destination_index ) = static_cast< UInt16Volume::VoxelType >( average_value + 1024 + 0.5 ) << 4;
                }
            }

            ++progress;
            if( progress >= progressEmitationThreshold )
            {
                emit progressed( progress );
                progress = 0;
            }

            QApplication::processEvents();
        }

    }

    emit finished();
}



}  // namespace Carna :: dicom

}  // namespace Carna
