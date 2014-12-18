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

#ifndef SERIESELEMENT_H_6014714286
#define SERIESELEMENT_H_6014714286

/** \file   SeriesElement.h
  * \brief  Defines \ref Carna::dicom::SeriesElement.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <string>

class QImage;

namespace Carna
{

namespace dicom
{

class Series;
class DicomImage;



// ----------------------------------------------------------------------------------
// SeriesElement
// ----------------------------------------------------------------------------------

/** \brief  Represents an element of \ref Series.
  *
  * \author Leonid Kostrykin
  * \date   1.10.12
  */
class CARNADICOM_LIB SeriesElement
{

    NON_COPYABLE

public:

    /** \brief  Instantiates with given arguments.
      */
    SeriesElement( const std::string&, double zPosition );

    /** \brief  Releases this \ref SeriesElement "series' elements".
      */
    ~SeriesElement();


    /** \brief  Holds the path of the represented DICOM image file.
      */
    const std::string fileName;

    /** \brief  Holds the z-position of the represented DICOM image.
      */
    const double zPosition;


    /** \brief  References the represented DICOM image.
      */
    const DicomImage& getDicomImage() const;

    /** \brief  Creates and returns new \c QImage object that contains the represented DICOM image.
      */
    QImage* createImage( unsigned int maxWidth, unsigned int maxHeight ) const;


    /** \cond 0
    */
    void setSeries( const Series& );
    /** \endcond
      */


private:

    mutable std::unique_ptr< DicomImage > dicomImage;

    const Series* series;

}; // SeriesElement



}  // namesapce Carna :: dicom

}  // namespace Carna

#endif // SERIESELEMENT_H_6014714286
