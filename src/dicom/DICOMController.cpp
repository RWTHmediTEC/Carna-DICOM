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

#include <Carna/dicom/DicomController.h>
#include <Carna/dicom/DicomControllerDetails.h>
#include <Carna/dicom/SeriesView.h>
#include <Carna/dicom/DicomManager.h>
#include <Carna/dicom/DicomExtractionSettings.h>
#include <Carna/dicom/Series.h>
#include <Carna/dicom/AsyncDirectory.h>
#include <QProgressDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QThread>
#include <QMessageBox>
#include <climits>
#include <sstream>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// DICOMController :: Details
// ----------------------------------------------------------------------------------

DICOMController::Details::Details( DICOMController& self )
    : self( self )
    , seriesView( new SeriesView() )
    , laSpacingZ( new QLabel( "Z-Spacing:" ) )
    , sbSpacingZ( new QDoubleSpinBox() )
    , buSaveIndex( new QPushButton( "Save Index..." ) )
    , buExtract( new QPushButton( "Extract Series..." ) )
    , buLoad( new QPushButton( "Load Series" ) )
    , workThread( new QThread() )
    , dir( new AsyncDirectory() )
{
    connect( workThread, SIGNAL( finished() ), workThread, SLOT( deleteLater() ) );
    connect( workThread, SIGNAL( finished() ),        dir, SLOT( deleteLater() ) );

    dir->moveToThread( workThread );
    workThread->start();

    connect( this, SIGNAL( openDirectory( const QString& ) ), dir, SLOT( open( const QString& ) ) );
}


DICOMController::Details::~Details()
{
    workThread->quit();
}



// ----------------------------------------------------------------------------------
// DICOMController
// ----------------------------------------------------------------------------------

DICOMController::DICOMController()
    : pimpl( new Details( *this ) )
{
    //qRegisterMetaType< Carna::dicom::DicomExtractionSettings >( "Carna::dicom::DicomExtractionSettings" );

 // ---------------------------------------------------------------------------------

    QVBoxLayout* const master = new QVBoxLayout();
    QHBoxLayout* const controls = new QHBoxLayout();

 // ---------------------------------------------------------------------------------

    QPushButton* const buOpenDirectory = new QPushButton( "Scan Directory..." );
    QPushButton* const buOpenIndex = new QPushButton( "Open Index..." );

    pimpl->sbSpacingZ->setRange( 0.01, std::numeric_limits< double >::max() );
    pimpl->sbSpacingZ->setSingleStep( 0.1 );
    pimpl->sbSpacingZ->setDecimals( 3 );
    pimpl->sbSpacingZ->setSuffix( " mm" );

    pimpl->laSpacingZ->setBuddy( pimpl->sbSpacingZ );
    pimpl->laSpacingZ->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

    controls->addWidget( buOpenDirectory );
    controls->addWidget( buOpenIndex );
    controls->addWidget( pimpl->buSaveIndex );
    controls->addWidget( pimpl->buExtract );
    controls->addWidget( pimpl->laSpacingZ );
    controls->addWidget( pimpl->sbSpacingZ );
    controls->addWidget( pimpl->buLoad );

    master->addLayout( controls );
    master->addWidget( pimpl->seriesView );

    connect( buOpenDirectory, SIGNAL( clicked() ), this, SLOT( openDirectory() ) );
    connect( buOpenIndex, SIGNAL( clicked() ), this, SLOT( openIndex() ) );
    connect( pimpl->buSaveIndex, SIGNAL( clicked() ), this, SLOT( saveIndex() ) );
    connect( pimpl->seriesView, SIGNAL( selectionChanged() ), this, SLOT( updateSelectionState() ) );
    connect( pimpl->buExtract, SIGNAL( clicked() ), this, SLOT( extractSeries() ) );
    connect( pimpl->buLoad, SIGNAL( clicked() ), this, SLOT( loadSeries() ) );

    connect
        ( pimpl->seriesView, SIGNAL( seriesSelected( const Carna::dicom::Series& ) )
        , this, SLOT( setSelectedSeries( const Carna::dicom::Series& ) ) );

    connect
        ( pimpl->seriesView, SIGNAL( seriesDoubleClicked( const Carna::dicom::Series& ) )
        , this, SLOT( loadSeries( const Carna::dicom::Series& ) ) );

    pimpl->buSaveIndex->setEnabled( false );
    pimpl->buExtract->setEnabled( false );
    updateSelectionState();

 // ---------------------------------------------------------------------------------

    this->setLayout( master );
}


DICOMController::~DICOMController()
{
}


void DICOMController::openDirectory()
{
    close();

 // ---------------------------------------------------------------------------------

    const QString dirName = QFileDialog::getExistingDirectory
        ( this
        , "Open Directory", ""
        , QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::HideNameFilterDetails );

    if( !dirName.isEmpty() )
    {
        QProgressDialog progress( dirName, "Cancel", 0, 100, this );
        connect( pimpl->dir, SIGNAL( fileCountDetermined( int ) ), &progress, SLOT( setMaximum( int ) ) );
        connect( pimpl->dir, SIGNAL( fileProcessed( int ) ), &progress, SLOT( setValue( int ) ) );
        connect( &progress, SIGNAL( canceled() ), pimpl->dir, SLOT( cancel() ) );
        emit pimpl->openDirectory( dirName );
        progress.exec();

        /* Report files that failed to be read.
         */
        if( !pimpl->dir->failedFiles().empty() )
        {
            std::stringstream failedFiles;
            for( auto it = pimpl->dir->failedFiles().begin(); it != pimpl->dir->failedFiles().end(); ++it )
            {
                failedFiles << ( *it ) << std::endl;
            }

            QMessageBox msgBox;
            msgBox.setIcon( QMessageBox::Warning );
            msgBox.setText
                ( "Failed to read "
                + QString::number( pimpl->dir->failedFiles().size() )
                + " file(s). Refer to the details section for a full list." );
            msgBox.setDetailedText( QString::fromStdString( failedFiles.str() ) );
            msgBox.setStandardButtons( QMessageBox::Ok );
            msgBox.setDefaultButton( QMessageBox::Ok );
            msgBox.setEscapeButton( QMessageBox::Ok );
            msgBox.exec();
        }

        /* Update UI if loading was not cancelled.
         */
        pimpl->seriesView->clear();
        for( auto patientItr = pimpl->dir->patients().begin(); patientItr != pimpl->dir->patients().end(); ++patientItr )
        {
            pimpl->seriesView->addPatient( **patientItr );
        }
        pimpl->buSaveIndex->setEnabled( true );
    }
}


void DICOMController::openIndex()
{
    close();

 // ----------------------------------------------------------------------------------

    const QString fileName = QFileDialog::getOpenFileName
        ( this
        , "Open Index", "", "Index Files (*.idx);;XML Files (*.xml);;All files (*.*)"
        , 0, QFileDialog::ReadOnly | QFileDialog::HideNameFilterDetails );

    if( !fileName.isEmpty() )
    {
    }
}


void DICOMController::saveIndex()
{
    const QString fileName = QFileDialog::getSaveFileName
        ( this
        , "Save Index", "", "Index Files (*.idx);;XML Files (*.xml);;All files (*.*)"
        , 0, QFileDialog::DontResolveSymlinks| QFileDialog::HideNameFilterDetails );

    if( !fileName.isEmpty() )
    {
    }
}


void DICOMController::close()
{
    pimpl->seriesView->clear();
    pimpl->buSaveIndex->setEnabled( false );
}


void DICOMController::updateSelectionState()
{
    const unsigned int selected_series_count = pimpl->seriesView->getSelectedSeries().size();
    pimpl->laSpacingZ->setEnabled( selected_series_count == 1 );
    pimpl->sbSpacingZ->setEnabled( selected_series_count == 1 );
    pimpl->buLoad->setEnabled( selected_series_count == 1 );
    pimpl->buExtract->setEnabled( selected_series_count > 0 );
}


void DICOMController::setSelectedSeries( const Series& series )
{
    pimpl->sbSpacingZ->setValue( series.spacingZ() );
}


void DICOMController::extractSeries()
{
    /*
    const auto& selected_series = seriesView->getSelectedSeries();
    if( selected_series.empty() )
    {
        return;
    }

    QString dirName = QFileDialog::getExistingDirectory( this
                                                       , "Extract Series"
                                                       , ""
                                                       , QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks );

    if( dirName.isEmpty() )
    {
        return;
    }

    QPushButton* const cancelButton = new QPushButton( "Cancel" );
    cancelButton->setEnabled( false );

    base::qt::CarnaProgressDialog extractingController( dirName, "Cancel", 0, 0, this );
    extractingController.setCancelButton( cancelButton );
    extractingController.setWindowTitle( "Extract Series" );
    extractingController.setWindowModality( Qt::WindowModal );
    extractingController.setModal( true );

    connect( manager, SIGNAL( finished() ), &extractingController, SLOT( accept() ) );
    connect( manager, SIGNAL( failed() ), &extractingController, SLOT( reject() ) );
    connect( manager, SIGNAL( failed( const QString& ) ), this, SLOT( extractionFailed( const QString& ) ) );
    connect( manager, SIGNAL( totalFilesCountChanged( unsigned int ) ), &extractingController, SLOT( setMaximum( unsigned int ) ) );
    connect( manager, SIGNAL( processedFilesCountChanged( unsigned int ) ), &extractingController, SLOT( setValue( unsigned int ) ) );

    DicomExtractionSettings settings( dirName, selected_series );

    emit extractSeries( settings );

    extractingController.exec();

    disconnect( manager, SIGNAL( failed( const QString& ) ), this, SLOT( extractionFailed( const QString& ) ) );
    */
}


void DICOMController::loadSeries()
{
    const auto& selectedSeries = pimpl->seriesView->getSelectedSeries();
    if( selectedSeries.size() == 1 )
    {
        const double zSpacing = pimpl->sbSpacingZ->value();
        //emit seriesLoadingRequested( SeriesLoadingRequest( **selectedSeries.begin(), zSpacing ) );
    }
}


void DICOMController::loadSeries( const Series& series )
{
    //emit seriesLoadingRequested( SeriesLoadingRequest( series, series.getSpacingZ() ) );
}



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
