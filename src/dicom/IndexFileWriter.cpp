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

#include <Carna/dicom/IndexFileWriter.h>
#include <Carna/dicom/IndexFile.h>
#include <Carna/base/CarnaException.h>
#include <QXmlStreamWriter>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// IndexFileWriter :: Details
// ----------------------------------------------------------------------------------

struct IndexFileWriter::Details
{
};



// ----------------------------------------------------------------------------------
// IndexFileWriter
// ----------------------------------------------------------------------------------

IndexFileWriter::IndexFileWriter()
    : pimpl( new Details() )
{
}


IndexFileWriter::~IndexFileWriter()
{
}


void IndexFileWriter::write( const QString& fileName, const std::vector< Patient* >& patients ) const
{
    QFile file( fileName );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
    {
        CARNA_FAIL( "Failed opening '" << fileName.toStdString() << "' for writing." );
    }

    QXmlStreamWriter xml( &file );
    xml.setAutoFormatting( true );
    xml.writeStartDocument();

    xml.writeStartElement( IndexFile::XML_DICOM_INDEX );

    for( auto patient_itr = patients.begin(); patient_itr != patients.end(); ++patient_itr )
    {
        const Patient& patient = **patient_itr;
        xml.writeStartElement( IndexFile::XML_PATIENT );
        xml.writeAttribute( IndexFile::XML_PATIENT_NAME, QString::fromStdString( patient.name ) );

        for( auto study_itr = patient.studies().begin(); study_itr != patient.studies().end(); ++study_itr )
        {
            const Study& study = **study_itr;
            xml.writeStartElement( IndexFile::XML_STUDY );
            xml.writeAttribute( IndexFile::XML_STUDY_NAME, QString::fromStdString( study.name ) );

            for( auto series_itr = study.series().begin(); series_itr != study.series().end(); ++series_itr )
            {
                const Series& series = **series_itr;
                xml.writeStartElement( IndexFile::XML_SERIES );
                xml.writeAttribute( IndexFile::XML_SERIES_NAME, QString::fromStdString( series.name ) );

                for( auto file_itr = series.elements().begin(); file_itr != series.elements().end(); ++file_itr )
                {
                    const SeriesElement& element = **file_itr;
                    xml.writeStartElement( IndexFile::XML_DICOM_IMAGE );
                    xml.writeAttribute( IndexFile::XML_DICOM_IMAGE_FILE, QString::fromStdString( element.fileName ) );
                    xml.writeAttribute( IndexFile::XML_DICOM_IMAGE_ZPOSITION, QString::number( element.zPosition ) );
                    xml.writeEndElement();
                }

                xml.writeEndElement();
            }

            xml.writeEndElement();
        }

        xml.writeEndElement();
    }

    xml.writeEndElement();
    xml.writeEndDocument();

    file.close();

    CARNA_ASSERT_EX( file.exists(), "Failed writing '" << fileName.toStdString() << "' for unknown reason." );
}



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
