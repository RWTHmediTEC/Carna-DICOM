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

#ifndef DICOMCONTROLLER_H_3294808493
#define DICOMCONTROLLER_H_3294808493

/** \file   DicomController.h
  * \brief  Defines \ref Carna::dicom::DicomController.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <Carna/helpers/VolumeGridHelper.h>
#include <QWidget>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// DICOMController
// ----------------------------------------------------------------------------------

/** \brief
  * Defines visual component for browsing, extracting and loading DICOM series.
  *
  * \image html DicomController01.png
  *
  * \author Leonid Kostrykin
  * \date   5.10.12 - 11.4.15
  */
class CARNADICOM_LIB DICOMController : public QWidget
{

    NON_COPYABLE
    Q_OBJECT

    class Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      */
    DICOMController();

    /** \brief
      * Deletes.
      */
    ~DICOMController();

    Carna::helpers::VolumeGridHelperBase* takeLoadedVolumeGrid();
    Carna::helpers::VolumeGridHelperBase::Spacing loadedVolumeGridSpacing() const;

public slots:

    void openDirectory();
    void openIndex();
    void close();
    void saveIndex();
    void extractSeries();
    void loadSeries( const Carna::dicom::Series& );
    void loadSeries();

signals:

    void volumeGridLoaded();

private slots:

    void updateSelectionState();
    void setSelectedSeries( const Carna::dicom::Series& );

}; // DICOMController



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // DICOMCONTROLLER_H_3294808493
