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

#include <Carna/base/model/SceneFactory.h>

namespace Carna
{

namespace dicom
{

class DicomSeries;



// ----------------------------------------------------------------------------------
// DicomSceneFactory
// ----------------------------------------------------------------------------------

/** \brief  Offers \ref Scene instantiating with little effort.
  *
  * All factory methods do throw the same exceptions.
  *
  * \ref AssertionFailure is thrown if at least one of the following pre-conditions
  * is not met:
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
class CARNA_EXPORT DicomSceneFactory : public SceneFactory
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit DicomSceneFactory
        ( QWidget* dialogParent = nullptr
        , const ParallelizationSettings& = ParallelizationSettings()
        , QObject* parent = nullptr );


public slots:

    /** \brief  Prompts the user for the data set to load using a \ref qt::DicomController dialog.
      */
    Scene* createFromUserInput();
    
    /** \brief  Dispatches the given request.
      *
      * Refer to the \ref qt::DicomController documentation for an example.
      */
    Scene* createFromRequest( const Carna::dicom::SeriesLoadingRequest& );


private:

    Scene* createFromDicomSeries( const DicomSeries& dicomSeries
                                , const Vector3ui& targetVolumeSize );

}; // DicomSceneFactory



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // DICOMSCENEFACTORY_H_6014714286