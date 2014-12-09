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

#ifndef STUDY_H_6014714286
#define STUDY_H_6014714286

/** \file   Study.h
  * \brief  Defines \ref Carna::dicom::Study.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <deque>

namespace Carna
{

namespace dicom
{

class Series;



// ----------------------------------------------------------------------------------
// Study
// ----------------------------------------------------------------------------------

/** \brief  Represents an unordered set of series.
  *
  * \author Leonid Kostrykin
  * \date   1.10.12
  */
class Study
{

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    explicit Study( const std::string& name );

    /** \brief  Releases acquired resources.
      */
    ~Study();


    const std::string name;


    const std::deque< Series* >& getSeries() const;

    void put( Series* );


private:

    std::deque< Series* > series;

}; // Study


inline Study& operator<<( Study& study, Series* series )
{
    study.put( series );
    return study;
}



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // STUDY_H_6014714286
