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

#ifndef INDEXFILEWRITER_H_3294808493
#define INDEXFILEWRITER_H_3294808493
#if !CARNAQT_DISABLED

/** \file   IndexFileWriter.h
  * \brief  Defines \ref Carna::dicom::IndexFileWriter.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/base/noncopyable.h>
#include <memory>
#include <vector>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// IndexFileWriter
// ----------------------------------------------------------------------------------

class IndexFileWriter
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    IndexFileWriter();
    virtual ~IndexFileWriter();

    void write( const QString& path, const std::vector< Patient* >& patients ) const;

}; // IndexFileWriter



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
#endif // INDEXFILEWRITER_H_3294808493
