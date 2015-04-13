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

#ifndef DICOMBROWSERDETAILS_H_3294808493
#define DICOMBROWSERDETAILS_H_3294808493
#if !CARNAQT_DISABLED

/** \file   DICOMBrowserDetails.h
  * \brief  Defines \ref Carna::dicom::DICOMBrowser implementation details.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/dicom/DICOMBrowser.h>
#include <Carna/dicom/AsyncVolumeGridFactory.h>
#include <Carna/dicom/AsyncDirectory.h>
#include <Carna/dicom/IndexFileReader.h>
#include <Carna/dicom/IndexFileWriter.h>
#include <QObject>

class QDoubleSpinBox;
class QPushButton;
class QCheckBox;
class QThread;
class QLabel;

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// DICOMBrowser :: Details
// ----------------------------------------------------------------------------------

class DICOMBrowser::Details : public QObject
{

    NON_COPYABLE
    Q_OBJECT

public:

    DICOMBrowser& self;
    Details( DICOMBrowser& self );
    virtual ~Details();

    SeriesView* const seriesView;

    QLabel* const laSpacingZ;
    QDoubleSpinBox* const sbSpacingZ;

    QPushButton* const buSaveIndex;
    QPushButton* const buExtract;
    QPushButton* const buLoad;

    QCheckBox* const cbNormals;

    QThread* const workThread;
    AsyncDirectory* const dir;
    AsyncVolumeGridFactory* const vgf;
    const std::unique_ptr< base::math::Vector3f > spacing;

    IndexFileReader ifr;
    IndexFileWriter ifw;

    const std::vector< Patient* >* patients;
    void setPatients( const std::vector< Patient* >& patients );

    void loadSeries( const Series& series );

signals:

    friend class DICOMBrowser;

    void openDirectory( const QString& path );
    void loadVolumeGrid();
    //void extractSeries( const Carna::dicom::DicomExtractionSettings& );

}; // DICOMBrowser :: Details



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
#endif // DICOMBROWSERDETAILS_H_3294808493
