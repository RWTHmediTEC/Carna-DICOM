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

#include <Carna/dicom/Directory.h>
#include <Carna/dicom/Patient.h>
#include <Carna/dicom/Study.h>
#include <Carna/dicom/Series.h>
#include <Carna/dicom/SeriesElement.h>
#include <Carna/base/Log.h>
#include <dicom-interface/dicomSeries.h>
#include <algorithm>
#include <regex>
#include <stack>

#ifdef _MSC_VER
#include <msvcpp/dirent.h>
#else
#include <dirent.h>
#endif

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// DirectoryInfo
// ----------------------------------------------------------------------------------

class DirectoryInfo
{

    NON_COPYABLE

public:

    static const char SEPARATOR = '/';
    static std::string normalizePath( const std::string& path );

    DirectoryInfo( const std::string& path );
    ~DirectoryInfo();

    DIR* const dir;
    const std::string path;

    bool isDirectory() const;
    std::string concat( const std::string& child ) const;

}; // DirectoryInfo


std::string DirectoryInfo::normalizePath( const std::string& path )
{
    /* Transform to unix-style path.
     */
    std::string unixoidPath = path;
    std::replace( unixoidPath.begin(), unixoidPath.end(), '\\', '/' );

    /* Remove tailing separators.
     */
    if( unixoidPath.empty() || unixoidPath == "/" || unixoidPath[ unixoidPath.length() - 1 ] != SEPARATOR )
    {
        return unixoidPath;
    }
    else
    {
        return unixoidPath.substr( 0, unixoidPath.length() - 1 );
    }
}


DirectoryInfo::DirectoryInfo( const std::string& pathArg )
    : dir( opendir( pathArg.c_str() ) )
    , path( normalizePath( pathArg ) )
{
}


DirectoryInfo::~DirectoryInfo()
{
    if( dir != NULL )
    {
        closedir( dir );
    }
}


bool DirectoryInfo::isDirectory() const
{
    return dir != NULL;
}


std::string DirectoryInfo::concat( const std::string& child ) const
{
    return path + SEPARATOR + child;
}



// ----------------------------------------------------------------------------------
// RecursiveFileIterator
// ----------------------------------------------------------------------------------

class RecursiveFileIterator : public DicomSeriesFileIterator
{

    std::stack< const DirectoryInfo* > dirs;
    const dirent* nextEntity;
    void fetchNextFile();

public:

    RecursiveFileIterator( const DirectoryInfo& dirInfo );
    virtual std::string getNextFile() override;
    virtual bool hasNextFile() override;

}; // RecursiveFileIterator


RecursiveFileIterator::RecursiveFileIterator( const DirectoryInfo& dirInfo )
    : nextEntity( NULL )
{
    dirs.push( &dirInfo );
    fetchNextFile();
}


void RecursiveFileIterator::fetchNextFile()
{
    /* The 'readdir' call from the DIRENT API gives a pointer to a memory block that
     * itself frees when it is called the next time or the directory is closed.
     */
    const DirectoryInfo& dirInfo = *dirs.top();
    std::string entityName;
    do
    {
        nextEntity = readdir( dirInfo.dir );
        if( nextEntity != NULL )
        {
            entityName = nextEntity->d_name;
        }
    }
    while( nextEntity != NULL && ( entityName == "." || entityName == ".." ) );
    
    /* Do not accept fetched entity if it refers to a directory.
     */
    if( nextEntity != NULL )
    {
        DirectoryInfo* const subdir = new DirectoryInfo( dirInfo.concat( entityName ) );
        if( subdir->isDirectory() )
        {
            dirs.push( subdir );
            nextEntity = NULL;
        }
    }

    /* Continue until either a file is found or all sub-directories were processed.
     */
    if( nextEntity == NULL && dirs.size() > 1 )
    {
        delete dirs.top();
        dirs.pop();
        fetchNextFile();
    }
}


std::string RecursiveFileIterator::getNextFile()
{
    CARNA_ASSERT( hasNextFile() );
    const DirectoryInfo& dirInfo = *dirs.top();
    const std::string nextFile = dirInfo.concat( nextEntity->d_name );
    fetchNextFile();
    return nextFile;
}


bool RecursiveFileIterator::hasNextFile()
{
    return nextEntity != NULL;
}



// ----------------------------------------------------------------------------------
// Directory :: Details
// ----------------------------------------------------------------------------------

struct Directory::Details : public DicomSeriesOpeningController
{
    Details();
    bool isOpen;
    std::string path;
    std::vector< Patient* > patients;
    std::map< std::string, Patient* > patientByName;
    Patient& patient( const std::string& name );

    void openDirectory( const DirectoryInfo& dirInfo );

    virtual void processNewImage
        ( const std::string& patientID
        , const std::string& studyID
        , const std::string& seriesID
        , const std::string& fileName
        , double zPosition ) override;

    Opener* opener;
    unsigned int totalFilesCount;

    virtual void dirOpened( unsigned int numFiles ) override;
    virtual void dirIterationProgress( unsigned int processedFilesCount ) override;
    virtual bool failedReadFile( const std::string& file, const std::vector< std::string >& succeeded ) override;
};


Directory::Details::Details()
    : isOpen( false )
{
}


Patient& Directory::Details::patient( const std::string& name )
{
    const auto itr = patientByName.find( name );
    if( itr == patientByName.end() )
    {
        Patient* const patient = new Patient( name );
        patientByName[ name ] = patient;
        patients.push_back( patient );
        return *patient;
    }
    else
    {
        return *itr->second;
    }
}


void Directory::Details::openDirectory( const DirectoryInfo& dirInfo )
{
    DicomSeries ds;
    RecursiveFileIterator fileItr( dirInfo );
    ds.openDicomDir( fileItr, *this );
}


void Directory::Details::processNewImage
    ( const std::string& patientID
    , const std::string& studyID
    , const std::string& seriesID
    , const std::string& fileName
    , double zPosition )
{
    Series& series = patient( patientID ).study( studyID ).series( seriesID );
    series.take( new SeriesElement( fileName, zPosition ) );
}


void Directory::Details::dirOpened( unsigned int numFiles )
{
    this->totalFilesCount = numFiles;
    this->dirIterationProgress( 0 );
}


void Directory::Details::dirIterationProgress( unsigned int processedFilesCount )
{
    opener->onProgress( processedFilesCount, totalFilesCount );
}


bool Directory::Details::failedReadFile( const std::string& file, const std::vector< std::string >& succeeded )
{
    base::Log::instance().record( base::Log::warning, "Failed to read DICOM file: " + file );
    opener->onFailure( file );
    return true;
}



// ----------------------------------------------------------------------------------
// Directory :: Opener
// ----------------------------------------------------------------------------------

Directory::Opener::Opener( Directory& dir )
    : dir( dir )
    , cancelled( false )
{
}


Directory::Opener::~Opener()
{
}


void Directory::Opener::cancel()
{
    dir.pimpl->abort();
    cancelled = true;
}


bool Directory::Opener::isCancelled() const
{
    return cancelled;
}


void Directory::Opener::open( const std::string& path )
{
    cancelled = false;
    dir.open( path, *this );
}



// ----------------------------------------------------------------------------------
// NullOpener
// ----------------------------------------------------------------------------------

struct NullOpener : public Directory::Opener
{
    NullOpener( Directory& dir );
    virtual void onProgress( unsigned int processedFilesCount, unsigned int totalFilesCount ) override;
    virtual void onFailure( const std::string& path ) override;
};


NullOpener::NullOpener( Directory& dir )
    : Directory::Opener( dir )
{
}


void NullOpener::onProgress( unsigned int processedFilesCount, unsigned int totalFilesCount )
{
}


void NullOpener::onFailure( const std::string& path )
{
}



// ----------------------------------------------------------------------------------
// Directory
// ----------------------------------------------------------------------------------

Directory::Directory()
    : pimpl( new Details() )
{
}


Directory::~Directory()
{
    close();
}


void Directory::close()
{
    if( pimpl->isOpen )
    {
        std::for_each( pimpl->patients.begin(), pimpl->patients.end(), std::default_delete< Patient >() );
        pimpl->patients.clear();
        pimpl->patientByName.clear();
        pimpl->isOpen = false;
    }
}


bool Directory::isOpen() const
{
    return pimpl->isOpen;
}


const std::vector< Patient* >& Directory::patients() const
{
    return pimpl->patients;
}


void Directory::open( const std::string& path )
{
    NullOpener opener( *this );
    open( path, opener );
}


void Directory::open( const std::string& path, Directory::Opener& opener )
{
    this->close();
    pimpl->opener = &opener;

    /* Open if 'path' refers to a directory.
     */
    DirectoryInfo dirInfo( path );
    CARNA_ASSERT( dirInfo.isDirectory() );
    pimpl->openDirectory( dirInfo );

    /* Denote that directory has been opened.
     */
    pimpl->isOpen = true;
}



}  // namespace Carna :: dicom

}  // namespace Carna
