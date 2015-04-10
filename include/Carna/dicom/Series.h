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

#ifndef SERIES_H_3294808493
#define SERIES_H_3294808493

/** \file   Series.h
  * \brief  Defines \ref Carna::dicom::Series.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <set>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// Series
// ----------------------------------------------------------------------------------

/** \brief
  * Represents a z-ordered set of DICOM files.
  *
  * \author Leonid Kostrykin
  * \date   1.10.12 - 9.4.15
  */
class CARNADICOM_LIB Series
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Defines partial \ref SeriesElement "series elements" ordering.
      */
    struct IsCloser
    {
        /** \brief
          * Tells whether `zPosition` of \a e1 is less than `zPosition` of \a e2.
          *
          * If the `zPosition` attribute of both elements is equal, the value
          * returned tells whether `fileName` of \a e1 is ordered lower than the
          * `fileName` of \a e2.
          */
        bool operator()( SeriesElement* e1, SeriesElement* e2 ) const;
    };

    /** \brief
      * Defines set of z-ordered \ref SeriesElement "series elements".
      */
    typedef std::set< SeriesElement*, IsCloser > OrderedElements;

    /** \brief
      * Minimal \ref SeriesElement "elements" number any series must have in order to
      * work properly.
      */
    const static unsigned int MINIMUM_ELEMENTS_COUNT = 2;

    /** \brief
      * Instantiates.
      */
    explicit Series( const std::string& name );

    /** \brief
      * Deletes.
      */
    ~Series();

    /** \brief
      * Holds the name of this series.
      */
    const std::string name;

    /** \brief
      * References z-ordered set of \ref SeriesElement "series elements".
      */
    const OrderedElements& elements() const;

    /** \brief
      * References the \ref SeriesElement "series element" which is most closest to
      * the volume's center.
      *
      * \pre `elements().size() >= MINIMUM_ELEMENTS_COUNT`
      */
    const SeriesElement& centralElement() const;

    /** \brief
      * Makes the \a seriesElements become part of this series.
      */
    void take( SeriesElement* seriesElement );

    /** \brief
      * Tells the z-spacing between two neighbored \ref SeriesElement "series elements".
      * \pre `elements().size() >= MINIMUM_ELEMENTS_COUNT`
      */
    double spacingZ() const;

    /** \brief
      * Tells whether this series already has an \ref SeriesElement "element" whose
      * DICOM image file is located at \a path.
      */
    bool contains( const std::string& path ) const;

}; // Series



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // SERIES_H_3294808493
