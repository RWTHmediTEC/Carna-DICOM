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

#include <Carna/dicom/IndexFileReader.h>
#include <Carna/dicom/IndexFile.h>
#include <Carna/base/CarnaException.h>
#include <QXmlSimpleReader>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// IndexFileReader :: Details
// ----------------------------------------------------------------------------------

struct IndexFileReader::Details
{
    std::vector< Patient* > patients;

    class IndexReader : public QXmlDefaultHandler
    {

    public:

        explicit IndexReader( std::map< std::string, Patient* >& patients )
            : patients( patients )
            , currentScope( INDICES )
            , currentPatient( nullptr )
            , currentStudy( nullptr )
            , currentSeries( nullptr )
        {
        }

        virtual bool startElement
            ( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& attributes )
        {
            switch( currentScope )
            {

            case INDICES:
                {
                    if( localName == IndexFile::XML_DICOM_INDEX )
                    {
                        currentScope = PATIENTS;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

            case PATIENTS:
                {
                    if( localName == IndexFile::XML_PATIENT )
                    {
                        currentPatient = new Patient( attributes.value( IndexFile::XML_PATIENT_NAME ).toStdString() );
                        patients[ currentPatient->name ] = currentPatient;

                        currentScope = STUDIES;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

            case STUDIES:
                {
                    if( localName == IndexFile::XML_STUDY )
                    {
                        CARNA_ASSERT( currentPatient != nullptr );

                        currentStudy = new Study( attributes.value( IndexFile::XML_STUDY_NAME ).toStdString() );
                        currentPatient->take( currentStudy );

                        currentScope = SERIES;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

            case SERIES:
                {
                    if( localName == IndexFile::XML_SERIES )
                    {
                        CARNA_ASSERT( currentPatient != nullptr );
                        CARNA_ASSERT( currentStudy != nullptr );

                        currentSeries = new Series( attributes.value( IndexFile::XML_SERIES_NAME ).toStdString() );
                        currentStudy->take( currentSeries );

                        currentScope = ELEMENTS;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

            case ELEMENTS:
                {
                    if( localName == IndexFile::XML_DICOM_IMAGE )
                    {
                        CARNA_ASSERT( currentPatient != nullptr );
                        CARNA_ASSERT( currentStudy != nullptr );
                        CARNA_ASSERT( currentSeries != nullptr );

                        bool ok = false;
                        const std::string fileName = attributes.value( IndexFile::XML_DICOM_IMAGE_FILE ).toStdString();
                        const double zPosition = attributes.value( IndexFile::XML_DICOM_IMAGE_ZPOSITION ).toDouble( &ok );

                        if( !ok )
                        {
                            return false;
                        }

                        SeriesElement* const element = new SeriesElement( fileName, zPosition );
                        currentSeries->take( element );

                        currentScope = ELEMENT;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

            }
            return true;
        }

        virtual bool endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
        {
            switch( currentScope )
            {

            case PATIENTS:
                {
                    if( localName == IndexFile::XML_DICOM_INDEX )
                    {
                        currentScope = INDICES;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

            case STUDIES:
                {
                    if( localName == IndexFile::XML_PATIENT )
                    {
                        currentScope = PATIENTS;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

            case SERIES:
                {
                    if( localName == IndexFile::XML_STUDY )
                    {
                        currentScope = STUDIES;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

            case ELEMENTS:
                {
                    if( localName == IndexFile::XML_SERIES )
                    {
                        currentScope = SERIES;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

            case ELEMENT:
                {
                    if( localName == IndexFile::XML_DICOM_IMAGE )
                    {
                        currentScope = ELEMENTS;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

            }
            return true;
        }

    private:

        unsigned int encounteredElements;
        std::map< std::string, Patient* >& patients;

        enum Scope
        {
            INDICES, PATIENTS, STUDIES, SERIES, ELEMENTS, ELEMENT
        }
        currentScope;

        Patient* currentPatient;
        Study* currentStudy;
        Series* currentSeries;

    }; // IndexReader
};



// ----------------------------------------------------------------------------------
// IndexFileReader
// ----------------------------------------------------------------------------------

IndexFileReader::IndexFileReader()
    : pimpl( new Details() )
{
}


IndexFileReader::~IndexFileReader()
{
    close();
}


void IndexFileReader::open( const QString& fileName )
{
    this->close();

    QFile file( fileName );
    if( !file.exists() )
    {
        std::stringstream ss;
        ss << "Failed opening '" << fileName.toStdString() << "' for reading.";
        CARNA_FAIL( ss.str() );
    }

    QXmlSimpleReader xmlReader;
    QXmlInputSource xmlSource( &file );

    std::map< std::string, Patient* > patients;
    Details::IndexReader parser( patients );
    xmlReader.setContentHandler( &parser );

    const bool xmlOk = xmlReader.parse( xmlSource );
    if( xmlOk )
    {
        for( auto itr = patients.begin(); itr != patients.end(); ++itr )
        {
            pimpl->patients.push_back( itr->second );
        }
    }
    else
    {
        std::stringstream ss;
        ss << "Failed reading '" << fileName.toStdString() << "' due to unexpected format.";
        CARNA_FAIL( ss.str() );
    }
}


void IndexFileReader::close()
{
    for( auto itr = pimpl->patients.begin(); itr != pimpl->patients.end(); ++itr )
    {
        delete *itr;
    }
    pimpl->patients.clear();
}


const std::vector< Patient* >& IndexFileReader::patients() const
{
    return pimpl->patients;
}



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
