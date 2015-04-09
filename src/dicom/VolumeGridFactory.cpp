/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/dicom/VolumeGridFactory.h>
#include <Carna/dicom/Series.h>
#include <Carna/dicom/SeriesElement.h>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// VolumeGridFactoryBase :: Details
// ----------------------------------------------------------------------------------

struct VolumeGridFactoryBase::Details
{
    Details();
    std::size_t maximumSegmentBytesize;
    const std::unique_ptr< base::math::Vector3f > spacing;
};


Details::Details()
    : maximumSegmentBytesize( helpers::VolumeGridHelperBase::DEFAULT_MAX_SEGMENT_BYTESIZE )
    , spacing( new base::math::Vector3f() )
{
}



// ----------------------------------------------------------------------------------
// VolumeGridFactoryBase
// ----------------------------------------------------------------------------------

VolumeGridFactoryBase::VolumeGridFactoryBase()
    : pimpl( new Details() )
{
}


void VolumeGridFactoryBase::setMaximumSegmentBytesize( std::size_t maximumSegmentBytesize )
{
    pimpl->maximumSegmentBytesize = maximumSegmentBytesize;
}


std::size_t VolumeGridFactoryBase::maximumSegmentBytesize() const
{
    return pimpl->maximumSegmentBytesize;
}

    
helpers::VolumeGridHelperBase* VolumeGridFactoryBase::loadSeries( const Series& series ) const
{
    pimpl->spacing->z() = series.spacingZ();
    helpers::VolumeGridHelperBase* result = nullptr;
    unsigned int zLast = static_cast< unsigned int >( -1 );
    for( auto seriesItr = series.elements().begin(); seriesItr != series.elements().end(); ++seriesItr )
    {
        const SeriesElement& element = **seriesItr;
        const DicomImage& slice = element.dicomImage();
        const unsigned int z = ++zLast;
        
        /* Initialize 'result' and 'pimpl->spacing' from the first image and verify
         * the consistency of the whole series otherwise.
         */
        const unsigned int width  = slice. getWidth();
        const unsigned int height = slice.getHeight();
        if( result == nullptr )
        {
            CARNA_ASSERT( slice.getWidth() > 0 && slice.getHeight() > 0 );
            result = this->create( base::math::Vector3ui( width, height, series.elements().size() ) );
            pimpl->spacing->x() = pimpl->spacing->y() = slice.getSpacingXY();
        }
        else
        {
            CARNA_ASSERT( width == result->nativeResolution.x() && height == result->nativeResolution.y() );
            CARNA_ASSERT( base::isEqual( pimpl->spacing->x(), slice.getSpacingXY() ) );
            CARNA_ASSERT( base::isEqual( pimpl->spacing->y(), slice.getSpacingXY() ) );
        }
        
        /* Copy HU pixel to voxel data.
         */
        for( unsigned int x = 0; x <  width; ++x )
        for( unsigned int y = 0; y < height; ++y )
        {
            const base::HUV huv = slice.getPixel( width - 1 - x, height - 1 - y );
            this->setHUVoxel( x, y, z, huv );
        }
    }
    return result;
}


base::Vector3f VolumeGridFactoryBase::spacing() const
{
    return *pimpl->spacing;
}



}  // namespace Carna :: dicom

}  // namespace Carna
