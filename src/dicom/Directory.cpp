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

#include <Carna/dicom/Directory.h>
#include <Carna/dicom/Patient.h>
#include <Carna/dicom/Study.h>
#include <Carna/dicom/Series.h>
#include <Carna/dicom/SeriesElement.h>
#include <Carna/base/Log.h>
#include <dicom-interface/dicomSeries.h>
#include <algorithm>
#include <regex>

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
// DIRENTFileIterator
// ----------------------------------------------------------------------------------

class DIRENTFileIterator : public DicomSeriesFileIterator
{

    DIR* const dir;
    const dirent* ent;

public:

    explicit DIRENTFileIterator( DIR* dir );
    virtual std::string getNextFile() override;
    virtual bool hasNextFile() override;

}; // DIRENTFileIterator


DIRENTFileIterator::DIRENTFileIterator( DIR* dir )
    : dir( dir )
{
    /* The 'readdir' call from the DIRENT API gives a pointer to a memory block that
     * itself frees when it is called the next time or the directory is closed.
     */
    ent = readdir( dir );
}


std::string DIRENTFileIterator::getNextFile()
{
    CARNA_ASSERT( hasNextFile() );
    const std::string nextFile = ent->d_name;
    ent = readdir( dir );
    return nextFile;
}


bool DIRENTFileIterator::hasNextFile()
{
    return ent != NULL;
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

    void openIndexFile( const std::string& path );
    void openDirectory( const std::string& path, DIR* dir );

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


void Directory::Details::openIndexFile( const std::string& path )
{
    base::Log::instance().record( base::Log::error, "Directory::Details::openIndexFile not implemented yet." );
}


void Directory::Details::openDirectory( const std::string& path, DIR* dir )
{
    DicomSeries ds;
    DIRENTFileIterator fileItr( dir );
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
{
}


Directory::Opener::~Opener()
{
}


void Directory::Opener::cancel()
{
    dir.pimpl->abort();
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

    /* Decide whether 'path' refers to an index file or to a directory.
     */
    DIR* const dir = opendir( path.c_str() );
    if( dir == NULL )
    {
        pimpl->openIndexFile( path );
    }
    else
    {
        pimpl->openDirectory( path, dir );
        closedir( dir );
    }

    /* Denote that directory has been opened.
     */
    pimpl->isOpen = true;
}



}  // namespace Carna :: dicom

}  // namespace Carna
