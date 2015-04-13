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

#ifndef DICOMBROWSER_H_3294808493
#define DICOMBROWSER_H_3294808493

/** \file   DICOMBrowser.h
  * \brief  Defines \ref Carna::dicom::DICOMBrowser.
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
// DICOMBrowser
// ----------------------------------------------------------------------------------

/** \brief
  * Defines visual component for browsing, extracting and loading DICOM series.
  *
  * \image html DICOMBrowser.png
  *
  * \author Leonid Kostrykin
  * \date   5.10.12 - 11.4.15
  */
class CARNADICOM_LIB DICOMBrowser : public QWidget
{

    NON_COPYABLE
    Q_OBJECT

    class Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      */
    DICOMBrowser();

    /** \brief
      * Deletes.
      */
    ~DICOMBrowser();

    /** \brief
      * Returns the previously loaded `VolumeGridHelper` object. The caller takes its
      * possession. Returns `nullptr` if nothing was loaded yet or the object already
      * has been taken.
      *
      * The \ref volumeGridLoaded signal fires when an object is available.
      */
    Carna::helpers::VolumeGridHelperBase* takeLoadedVolumeGrid();

    /** \brief
      * Tells the spacing of the `VolumeGridHelper` object \ref loadSeries "loaded"
      * latest. The return value of this method is undefined if \ref loadSeries was
      * not called yet.
      */
    Carna::helpers::VolumeGridHelperBase::Spacing loadedVolumeGridSpacing() const;

public slots:

    void openDirectory();   ///< Prompts the user to select a filesystem directory that will be scanned for DICOM series.
    void openIndex();       ///< Prompts the user to select an index file that lists DICOM series.
    void closePatients();   ///< Closes any previously loaded DICOM series.
    void saveIndex();       ///< Prompts the user to select an index file that the currently loaded DICOM series will be saved to.
    void extractSeries();   ///< Not implemented yet.
    
    /** \brief
      * Creates an `VolumeGridHelper` object from \a series asynchronously.
      */
    void loadSeries( const Carna::dicom::Series& series );

    /** \brief
      * Creates an `VolumeGridHelper` object from the currently selected DICOM
      * series asynchronously.
      *
      * Nothing happens if no series is selected at the moment.
      */
    void loadSeries();

signals:

    /** \brief
      * Fires when a new `VolumeGridHelper` object is ready
      * \ref takeLoadedVolumeGrid "to be taken".
      */
    void volumeGridLoaded();

private slots:

    void updateSelectionState();
    void setSelectedSeries( const Carna::dicom::Series& );

}; // DICOMBrowser



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // DICOMBROWSER_H_3294808493
