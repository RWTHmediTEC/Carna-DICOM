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

#ifndef INDEXFILEREADER_H_3294808493
#define INDEXFILEREADER_H_3294808493
#if !CARNAQT_DISABLED

/** \file   IndexFileReader.h
  * \brief  Defines \ref Carna::dicom::IndexFileReader.
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
// IndexFileReader
// ----------------------------------------------------------------------------------

class IndexFileReader
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    IndexFileReader();
    virtual ~IndexFileReader();

    const std::vector< Patient* >& patients() const;

    void open( const QString& path );
    void close();

}; // IndexFileReader



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
#endif // INDEXFILEREADER_H_3294808493
