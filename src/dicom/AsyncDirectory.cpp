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

#include <Carna/dicom/CarnaDICOM.h>
#if !CARNAQT_DISABLED

#include <Carna/dicom/AsyncDirectory.h>
#include <Carna/dicom/Directory.h>
#include <Carna/base/CarnaException.h>
#include <QEventLoop>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// AsyncDirectory :: Details
// ----------------------------------------------------------------------------------

struct AsyncDirectory::Details
{
    Directory dir;
    std::vector< std::string > failedFiles;

    struct Opener : public Directory::Opener
    {
        AsyncDirectory& self;
        int totalFilesCount;
        Opener( AsyncDirectory& self, Directory& dir );
        virtual void onProgress( unsigned int processedFilesCount, unsigned int totalFilesCount ) override;
        virtual void onFailure( const std::string& path ) override;
    };

    Details();
    Opener* opener;
};


AsyncDirectory::Details::Details()
    : opener( nullptr )
{
}


// ----------------------------------------------------------------------------------
// AsyncDirectory :: Details :: Opener
// ----------------------------------------------------------------------------------

AsyncDirectory::Details::Opener::Opener( AsyncDirectory& self, Directory& dir )
    : Directory::Opener( dir )
    , self( self )
    , totalFilesCount( -1 )
{
}


void AsyncDirectory::Details::Opener::onProgress( unsigned int processedFilesCount, unsigned int totalFilesCount )
{
    QEventLoop eventLoop;
    eventLoop.processEvents();
    if( !isCancelled() )
    {
        const int newTotalFilesCount = static_cast< int >( totalFilesCount );
        if( this->totalFilesCount != newTotalFilesCount )
        {
            this->totalFilesCount = newTotalFilesCount;
            emit self.fileCountDetermined( this->totalFilesCount );
        }
        emit self.fileProcessed( processedFilesCount );
    }
}


void AsyncDirectory::Details::Opener::onFailure( const std::string& path )
{
    self.pimpl->failedFiles.push_back( path );
}



// ----------------------------------------------------------------------------------
// AsyncDirectory
// ----------------------------------------------------------------------------------

AsyncDirectory::AsyncDirectory()
    : pimpl( new Details() )
{
}


AsyncDirectory::~AsyncDirectory()
{
}


void AsyncDirectory::open( const QString& path )
{
    CARNA_ASSERT_EX( pimpl->opener == nullptr, "AsyncDirectory::open may only be executed once per object at a time!" );
    this->close();
    Details::Opener opener( *this, pimpl->dir );
    pimpl->opener = &opener;
    pimpl->opener->open( path.toStdString() );
    pimpl->opener = nullptr;
}


void AsyncDirectory::close()
{
    pimpl->dir.close();
    pimpl->failedFiles.clear();
}


void AsyncDirectory::cancel()
{
    if( pimpl->opener != nullptr )
    {
        pimpl->opener->cancel();
    }
}


const std::vector< std::string >& AsyncDirectory::failedFiles() const
{
    return pimpl->failedFiles;
}


const std::vector< Patient* >& AsyncDirectory::patients() const
{
    return pimpl->dir.patients();
}



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
