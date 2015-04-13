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

#ifndef PATIENT_H_3294808493
#define PATIENT_H_3294808493

/** \file   Patient.h
  * \brief  Defines \ref Carna::dicom::Patient.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/base/noncopyable.h>
#include <vector>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// Patient
// ----------------------------------------------------------------------------------

/** \brief
  * Represents an unordered set of \ref Study "studies".
  *
  * \author Leonid Kostrykin
  * \date   1.10.12 - 10.4.15
  */
class Patient
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      */
    explicit Patient( const std::string& name );

    /** \brief
      * Deletes studies.
      */
    ~Patient();

    /** \brief
      * Tells the name of this patient.
      */
    const std::string name;

    /** \brief
      * Tells the studies associated with this patient.
      */
    const std::vector< Study* >& studies() const;

    /** \brief
      * Enlists \a study in \ref studies and takes its possession.
      */
    void take( Study* study );

    /** \brief
      * References the study named \a name associated with this patient. Creates a
      * new empty study when no such study is associated with this patient yet.
      */
    Study& study( const std::string& name );

}; // Patient



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // PATIENT_H_3294808493
