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

#ifndef ASYNCDIRECTORY_H_3294808493
#define ASYNCDIRECTORY_H_3294808493
#if !CARNAQT_DISABLED

/** \file   AsyncDirectory.h
  * \brief  Defines \ref Carna::dicom::AsyncDirectory.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/base/noncopyable.h>
#include <QObject>
#include <memory>
#include <vector>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// AsyncDirectory
// ----------------------------------------------------------------------------------

class AsyncDirectory : public QObject
{

    NON_COPYABLE
    Q_OBJECT

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    AsyncDirectory();
    virtual ~AsyncDirectory();

    const std::vector< std::string >& failedFiles() const;
    const std::vector< Patient* >& patients() const;

public slots:

    void open( const QString& path );
    void close();
    void cancel();

signals:

    void fileCountDetermined( int totalFilesCount );
    void fileProcessed( int processedFilesCount );

}; // AsyncDirectory



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
#endif // ASYNCDIRECTORY_H_3294808493
