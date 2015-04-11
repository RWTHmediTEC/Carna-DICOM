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

#ifndef DIRECTORY_H_3294808493
#define DIRECTORY_H_3294808493

/** \file   Directory.h
  * \brief  Defines \ref Carna::dicom::Directory.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/base/noncopyable.h>
#include <vector>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// Directory
// ----------------------------------------------------------------------------------

/** \brief
  * Loads and maintains an unordered set of \ref Patient "patients".
  *
  * \author Leonid Kostrykin
  * \date   10.4.15
  */
class Directory
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    class Opener
    {
        Directory& dir;

    public:

        Opener( Directory& );
        virtual ~Opener();
        virtual void onProgress( unsigned int processedFilesCount, unsigned int totalFilesCount ) = 0;
        virtual void onFailure( const std::string& ) = 0;
        void cancel();
    };

    /** \brief
    * Instantiates.
    * \post `isOpen() == false`
      */
    Directory();

    /** \brief
      * Deletes patients.
      */
    ~Directory();

    void open( const std::string& path );

    void open( const std::string& path, Opener& opener );

    void close();

    bool isOpen() const;

    const std::vector< Patient* >& patients() const;

}; // Directory



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // DIRECTORY_H_3294808493
