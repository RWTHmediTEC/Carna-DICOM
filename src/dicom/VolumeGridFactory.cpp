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


VolumeGridFactoryBase::Details::Details()
    : maximumSegmentBytesize( helpers::VolumeGridHelperBase::DEFAULT_MAX_SEGMENT_BYTESIZE )
    , spacing( new base::math::Vector3f() )
{
}



// ----------------------------------------------------------------------------------
// VolumeGridFactoryBase :: Progress
// ----------------------------------------------------------------------------------

VolumeGridFactoryBase::Progress::~Progress()
{
}



// ----------------------------------------------------------------------------------
// VolumeGridFactoryBase :: NullProgress
// ----------------------------------------------------------------------------------

struct NullProgress : public VolumeGridFactoryBase::Progress
{
    virtual void setTotalSlicesCount( unsigned int ) override;
    virtual void setProcessedSlicesCount( unsigned int ) override;
};


void NullProgress::setTotalSlicesCount( unsigned int )
{
}


void NullProgress::setProcessedSlicesCount( unsigned int )
{
}



// ----------------------------------------------------------------------------------
// VolumeGridFactoryBase
// ----------------------------------------------------------------------------------

VolumeGridFactoryBase::VolumeGridFactoryBase()
    : pimpl( new Details() )
{
}


VolumeGridFactoryBase::~VolumeGridFactoryBase()
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


helpers::VolumeGridHelperBase* VolumeGridFactoryBase::loadSeries( const Series& series )
{
    NullProgress progress;
    return loadSeries( series, progress );
}


helpers::VolumeGridHelperBase* VolumeGridFactoryBase::loadSeries( const Series& series, Progress& progress )
{
    pimpl->spacing->z() = series.spacingZ();
    helpers::VolumeGridHelperBase* result = nullptr;
    unsigned int zLast = static_cast< unsigned int >( -1 );
    progress.setTotalSlicesCount( series.elements().size() - 1 );
    progress.setProcessedSlicesCount( 0 );
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
            const float spacingXY = static_cast< float >( slice.getSpacingXY() );
            CARNA_ASSERT( width == result->nativeResolution.x() && height == result->nativeResolution.y() );
            CARNA_ASSERT( base::math::isEqual( pimpl->spacing->x(), spacingXY ) );
            CARNA_ASSERT( base::math::isEqual( pimpl->spacing->y(), spacingXY ) );
        }
        
        /* Copy HU pixel to voxel data.
         */
        base::math::Vector3ui location( 0, 0, z );
        for( location.y() = 0; location.y() < height; ++location.y() )
        for( location.x() = 0; location.x() <  width; ++location.x() )
        {
            const base::HUV huv = const_cast< DicomImage& >( slice ).getPixel( width - 1 - location.x(), height - 1 - location.y() );
            this->setHUVoxel( location, huv );
        }

        /* Denote progress.
         */
        progress.setProcessedSlicesCount( z );
    }
    return result;
}


const base::math::Vector3f& VolumeGridFactoryBase::spacing() const
{
    return *pimpl->spacing;
}



}  // namespace Carna :: dicom

}  // namespace Carna
