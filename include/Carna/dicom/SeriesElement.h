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

#ifndef SERIESELEMENT_H_3294808493
#define SERIESELEMENT_H_3294808493

/** \file   SeriesElement.h
  * \brief  Defines \ref Carna::dicom::SeriesElement.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <string>

#if CARNAQT_ENABLED
class QImage;
#endif

namespace Carna
{

namespace dicom
{



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
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      */
    SeriesElement( const std::string& fileName, double zPosition );

    /** \brief
      * Deletes series' elements.
      */
    ~SeriesElement();

    /** \brief
      * Holds the path of the represented DICOM image file.
      */
    const std::string fileName;

    /** \brief
      * Holds the z-position of the represented DICOM image.
      */
    const double zPosition;

    /** \brief
      * References the represented DICOM image.
      */
    const DicomImage& dicomImage() const;

#if CARNAQT_ENABLED
    /** \brief  Creates and returns new \c QImage object that contains the represented DICOM image.
      */
    QImage* createImage( unsigned int maxWidth, unsigned int maxHeight ) const;
#endif

    void putInto( Series& series );

}; // SeriesElement



}  // namesapce Carna :: dicom

}  // namespace Carna

#endif // SERIESELEMENT_H_3294808493
