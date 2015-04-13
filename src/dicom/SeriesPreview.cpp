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

#include <Carna/dicom/CarnaDICOM.h>
#if !CARNAQT_DISABLED

#include <Carna/dicom/SeriesPreview.h>
#include <Carna/dicom/Series.h>
#include <Carna/dicom/SeriesElement.h>
#include <Carna/base/CarnaException.h>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QVBoxLayout>
#include <dicom-interface/dicomImage.h>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// Types & Globals
// ----------------------------------------------------------------------------------

const static unsigned int PREVIEW_SIZE = 128;



// ----------------------------------------------------------------------------------
// SeriesPreview :: Details
// ----------------------------------------------------------------------------------

struct SeriesPreview::Details
{
    Details();
    const Series* series;
    QLabel* const image;
    QLabel* const caption;
};


SeriesPreview::Details::Details()
    : series( nullptr )
    , image( new QLabel() )
    , caption( new QLabel() )
{
}



// ----------------------------------------------------------------------------------
// SeriesPreview
// ----------------------------------------------------------------------------------

SeriesPreview::SeriesPreview( QWidget* parent )
    : QFrame( parent )
    , pimpl( new Details() )
{
    this->setLayout( new QVBoxLayout() );
    this->layout()->addWidget( pimpl->image );
    this->layout()->addWidget( pimpl->caption );
    this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    this->setStyleSheet( "Carna--SeriesPreview{ border: 1px solid #FFFFFF; background-color: black; }" );

    pimpl->image->setAlignment( Qt::AlignHCenter );
    pimpl->caption->setAlignment( Qt::AlignHCenter );
    pimpl->caption->setStyleSheet( "color: #FFFFFF; background: none;" );
}


SeriesPreview::~SeriesPreview()
{
}


void SeriesPreview::setSeries( const Series& series )
{
    pimpl->series = &series;

    const DicomImage& dicomImage = series.centralElement().dicomImage();
    const unsigned int dicomImageSize = std::max( dicomImage.getWidth(), dicomImage.getHeight() );
    const unsigned int previewWidth = ( dicomImage.getWidth() * PREVIEW_SIZE ) / dicomImageSize;
    const unsigned int previewHeight = ( dicomImage.getHeight() * PREVIEW_SIZE ) / dicomImageSize;

    std::unique_ptr< QImage > image( series.centralElement().createImage( previewWidth, previewHeight ) );

    QPixmap pixmap;
    pixmap.convertFromImage( *image );

    const static unsigned int max_caption_text_length = 20;
    const QString caption_text = series.name.length() < max_caption_text_length
        ? QString::fromStdString( series.name )
        : QString::fromStdString( series.name.substr( 0, max_caption_text_length - 3 ) ) + "...";

    pimpl->image->setPixmap( pixmap );
    pimpl->caption->setText( QString::number( series.elements().size() ) + " images\n" + caption_text );
    this->setToolTip( QString::fromStdString( series.name ) );
}


const Series& SeriesPreview::series() const
{
    CARNA_ASSERT( pimpl->series != nullptr );
    return *pimpl->series;
}


bool SeriesPreview::hasSeries() const
{
    return pimpl->series != nullptr;
}



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
