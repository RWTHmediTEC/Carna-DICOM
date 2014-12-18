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

#ifndef DICOMSCENEFACTORY_H_6014714286
#define DICOMSCENEFACTORY_H_6014714286

/** \file   DicomSceneFactory.h
  * \brief  Defines \ref Carna::dicom::DicomSceneFactory.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/base/model/SceneFactory.h>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// DicomSceneFactory
// ----------------------------------------------------------------------------------

/** \brief  Instantiates \c Carna::base::model::Scene from DICOM series with little effort.
  *
  * All factory methods do throw the same exceptions.
  *
  * \c Carna::base::AssertionFailure is thrown if at least one of the following pre-conditions is not met:
  *
  * - The requested number of parallelized workers is not supported by implementation.
  * - The requested target volume size is larger than the original data.
  * - The requested target volume size is lower than 2 pixels along at least one axis.
  *
  * \c std::runtime_error is thrown when at least one of these cases applies:
  *
  * - The DICOM image size is not consistent.
  *
  * \author Leonid Kostrykin
  * \date   26.10.11 - 21.2.13
  */
class CARNADICOM_LIB DicomSceneFactory : public base::model::SceneFactory
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit DicomSceneFactory
        ( QWidget* dialogParent = nullptr
        , const base::ParallelizationSettings& = base::ParallelizationSettings()
        , QObject* parent = nullptr );


public slots:

    /** \brief  Prompts the user for the data set to load using a \ref DicomController dialog.
      */
    base::model::Scene* createFromUserInput();
    
    /** \brief  Dispatches the given request.
      *
      * Refer to the \ref DicomController documentation for an example.
      */
    base::model::Scene* createFromRequest( const Carna::dicom::SeriesLoadingRequest& );


private:

    base::model::Scene* createFromDicomSeries
        ( const DicomSeries& dicomSeries
        , const base::Vector3ui& targetVolumeSize );

}; // DicomSceneFactory



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // DICOMSCENEFACTORY_H_6014714286
