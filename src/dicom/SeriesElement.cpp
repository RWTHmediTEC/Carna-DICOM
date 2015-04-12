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

#include <Carna/dicom/SeriesElement.h>
#include <Carna/dicom/Series.h>
#include <Carna/base/math.h>
#include <Carna/base/CarnaException.h>
#include <dicom-interface/dicomImage.h>
#include <algorithm>

#if CARNAQT_ENABLED
#include <QImage>
#include <QColor>
#endif

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// SeriesElement :: Details
// ----------------------------------------------------------------------------------

struct SeriesElement :: Details
{
    Details();
    std::unique_ptr< DicomImage > dicomImage;
    const Series* series;
};


SeriesElement::Details::Details()
    : series( nullptr )
{
}



// ----------------------------------------------------------------------------------
// SeriesElement
// ----------------------------------------------------------------------------------

SeriesElement::SeriesElement( const std::string& fileName, double zPosition )
    : pimpl( new Details() )
    , fileName( fileName )
    , zPosition( zPosition )
{
}


SeriesElement::~SeriesElement()
{
}


const DicomImage& SeriesElement::dicomImage() const
{
    CARNA_ASSERT( pimpl->series != nullptr );
    if( pimpl->dicomImage.get() == nullptr )
    {
        pimpl->dicomImage.reset( new DicomImage( fileName ) );
        pimpl->dicomImage->setPositionZ( this->zPosition );
        pimpl->dicomImage->setSpacingZ( pimpl->series->spacingZ() );
    }

    return *pimpl->dicomImage;
}


#if CARNAQT_ENABLED
QImage* SeriesElement::createImage( unsigned int max_width, unsigned int max_height ) const
{
    const DicomImage& dicomImage = this->dicomImage();

    const unsigned int width = std::min( static_cast< unsigned >( dicomImage.getWidth() ), max_width );
    const unsigned int height = std::min( static_cast< unsigned >( dicomImage.getHeight() ), max_height );

    QImage* const image = new QImage( width, height, QImage::Format_Indexed8 );

    QVector< QRgb > colorTable( 256 );
    for( unsigned int i = 0; i < 256; ++i )
    {
        colorTable[ i ] = qRgb( i, i, i );
    }
    image->setColorCount( 256 );
    image->setColorTable( colorTable );

    double dx = static_cast< double >( dicomImage.getWidth() ) / width;
    double dy = static_cast< double >( dicomImage.getHeight() ) / height;

    double src_x = 0;
    for( unsigned int dst_x = 0; dst_x < width; ++dst_x, src_x += dx )
    {
        double src_y = 0;
        for( unsigned int dst_y = 0; dst_y < height; ++dst_y, src_y += dy )
        {
            const unsigned int src_x_int = std::min( static_cast< unsigned int >( src_x + 0.5 ), static_cast< unsigned int >( dicomImage.getWidth() - 1 ) );
            const unsigned int src_y_int = std::min( static_cast< unsigned int >( src_y + 0.5 ), static_cast< unsigned int >( dicomImage.getHeight() - 1 ) );

            const int huv = const_cast< DicomImage& >( dicomImage ).getPixel( src_x_int, src_y_int );
            const int i = base::math::clamp< int >( ( ( huv + 1024 ) * 256 ) / 4096, 0, 255 );
            
            image->setPixel( dst_x, dst_y, i );
        }
    }

    return image;
}
#endif


void SeriesElement::putInto( Series& series )
{
    if( &series != pimpl->series )
    {
        CARNA_ASSERT_EX( pimpl->series == nullptr, "This element already belongs to another series." );
        pimpl->series = &series;
        series.take( this );
    }
}



}  // namespace Carna :: dicom

}  // namespace Carna
