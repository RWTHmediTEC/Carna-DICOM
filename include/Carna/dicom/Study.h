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

#ifndef STUDY_H_3294808493
#define STUDY_H_3294808493

/** \file   Study.h
  * \brief  Defines \ref Carna::dicom::Study.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/base/noncopyable.h>
#include <vector>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// Study
// ----------------------------------------------------------------------------------

/** \brief
  * Represents an unordered set of \ref Series "series".
  *
  * \author Leonid Kostrykin
  * \date   1.10.12 - 10.4.15
  */
class Study
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      */
    explicit Study( const std::string& name );

    /** \brief
      * Deletes series.
      */
    ~Study();

    const std::string name;

    const std::vector< Series* >& series() const;

    void put( Series* );

}; // Study


inline Study& operator<<( Study& study, Series* series )
{
    study.put( series );
    return study;
}



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // STUDY_H_3294808493
