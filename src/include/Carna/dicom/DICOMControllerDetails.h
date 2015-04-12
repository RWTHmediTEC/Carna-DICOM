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

#ifndef DICOMCONTROLLERDETAILS_H_3294808493
#define DICOMCONTROLLERDETAILS_H_3294808493
#if !CARNAQT_DISABLED

/** \file   DICOMControllerDetails.h
  * \brief  Defines \ref Carna::dicom::DICOMController implementation details.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/dicom/DICOMController.h>
#include <Carna/dicom/AsyncDirectory.h>
#include <Carna/dicom/IndexFileReader.h>
#include <QObject>

class QDoubleSpinBox;
class QPushButton;
class QThread;
class QLabel;

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// DICOMController :: Details
// ----------------------------------------------------------------------------------

class DICOMController::Details : public QObject
{

    NON_COPYABLE
    Q_OBJECT

public:

    DICOMController& self;
    Details( DICOMController& self );
    virtual ~Details();

    SeriesView* const seriesView;

    QLabel* const laSpacingZ;
    QDoubleSpinBox* const sbSpacingZ;

    QPushButton* const buSaveIndex;
    QPushButton* const buExtract;
    QPushButton* const buLoad;

    QThread* const workThread;
    AsyncDirectory* const dir;

    IndexFileReader ifr;

    void onPatientsLoaded( const std::vector< Patient* >& patients );

signals:

    friend class DICOMController;

    void openDirectory( const QString& path );
    //void extractSeries( const Carna::dicom::DicomExtractionSettings& );

}; // DICOMController :: Details



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
#endif // DICOMCONTROLLERDETAILS_H_3294808493
