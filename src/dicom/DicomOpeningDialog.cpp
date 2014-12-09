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

#include <Carna/dicom/DicomOpeningDialog.h>
#include <Carna/dicom/DicomController.h>
#include <QVBoxLayout>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// DicomOpeningDialog
// ----------------------------------------------------------------------------------

DicomOpeningDialog::DicomOpeningDialog( QWidget* parent )
    : QDialog( parent )
{
    this->setLayout( new QVBoxLayout() );
    this->layout()->setContentsMargins( 0, 0, 0, 0 );

    DicomController* const controller = new DicomController();

    this->layout()->addWidget( controller );
    controller->setMinimumHeight( 400 );

    connect( controller, SIGNAL( seriesLoadingRequested( const Carna::dicom::SeriesLoadingRequest& ) ),
        this, SLOT( loadingRequested( const Carna::dicom::SeriesLoadingRequest& ) ) );
}


DicomOpeningDialog::~DicomOpeningDialog()
{
}


void DicomOpeningDialog::loadingRequested( const SeriesLoadingRequest& request )
{
    this->request = request;
    this->accept();
}


const SeriesLoadingRequest& DicomOpeningDialog::getRequest() const
{
    return this->request;
}



}  // namespace Carna :: dicom

}  // namespace Carna
