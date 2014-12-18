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

#ifndef SERIESLOADINGREQUEST_H_6014714286
#define SERIESLOADINGREQUEST_H_6014714286

/** \file   SeriesLoadingRequest.h
  * \brief  Defines \ref Carna::dicom::SeriesLoadingRequest.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/Carna.h>

namespace Carna
{

namespace dicom
{

class Series;



// ----------------------------------------------------------------------------------
// SeriesLoadingRequest
// ----------------------------------------------------------------------------------

/** \brief  Represents request to instantiate \c Carna::base::model::Scene from DICOM series.
  *
  * \see    \ref DicomController, \ref DicomSceneFactory
  *
  * \author Leonid Kostrykin
  * \date   5.10.12 - 8.18.14
  */
class CARNADICOM_LIB SeriesLoadingRequest
{

public:

    /** \brief  Instantiates an empty request.
      */
    SeriesLoadingRequest()
    {
    }
    
    /** \brief  Instantiates request to create \c Carna::base::model::Scene from given DICOM series and given spacing.
      */
    SeriesLoadingRequest( const Series& series, double zSpacing )
        : series( &series )
        , zSpacing( zSpacing )
    {
    }

    
    /** \brief  Sets the DICOM series a \c Carna::base::model::Scene shall be created from by this request.
      */
    void setSeries( const Series& series )
    {
        this->series = &series;
    }

    /** \brief  References the DICOM series a \c Carna::base::model::Scene shall be created from by this request.
      */
    const Series& getSeries() const
    {
        return *series;
    }

    /** \brief  Sets the distance between two neighbored DICOM images in millimeters.
      */
    void setSpacingZ( double zSpacing )
    {
        this->zSpacing = zSpacing;
    }
    
    /** \brief  Tells the distance between two neighbored DICOM images in millimeters.
      */
    double getSpacingZ() const
    {
        return zSpacing;
    }


private:

    const Series* series;

    double zSpacing;

}; // SeriesLoadingRequest



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // SERIESLOADINGREQUEST_H_6014714286
