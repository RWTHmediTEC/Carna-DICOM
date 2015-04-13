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

    /** \brief
      * Defines interface for \ref Directory opening processes with callbacks.
      */
    class Opener
    {
        Directory& dir;
        bool cancelled;

    public:

        /** \brief
          * Instantiates.
          */
        Opener( Directory& );

        /** \brief
          * Deletes.
          */
        virtual ~Opener();

        /** \brief
          * Notifies about directory scanning progress.
          */
        virtual void onProgress( unsigned int processedFilesCount, unsigned int totalFilesCount ) = 0;

        /** \brief
          * Notifies that \a file could not be read like a DICOM image.
          */
        virtual void onFailure( const std::string& file ) = 0;

        /** \brief
          * Cancels the process.
          */
        void cancel();

        /** \brief
          * Tells whether this has been cancelled.
          */
        bool isCancelled() const;

        /** \brief
          * Scans \a path for DICOM series.
          */
        void open( const std::string& path );
    };

    /** \brief
    * Instantiates.
    * \post `isOpen() == false`
      */
    Directory();

    /** \brief
      * Deletes patients and \ref close "closes".
      */
    ~Directory();

    /** \brief
      * Scans \a path for DICOM series.
      * \post `isOpen() == true`
      *
      * The \ref Opener class provides an interface for opening processes with
      * callbacks.
      */
    void open( const std::string& path );

    /** \brief
      * Closes any previously opened directories.
      * \post `isOpen() == false`
      */
    void close();

    /** \brief
      * Tells whether a directory is open currently.
      */
    bool isOpen() const;

    /** \brief
      * References the patients from the currently open directory.
      * \pre `isOpen() == true`
      */
    const std::vector< Patient* >& patients() const;

private:

    void open( const std::string& path, Opener& opener );

}; // Directory



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // DIRECTORY_H_3294808493
