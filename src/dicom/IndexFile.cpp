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

#include <Carna/dicom/IndexFile.h>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// IndexFile
// ----------------------------------------------------------------------------------

const QString IndexFile::XML_DICOM_INDEX = "DicomIndex";
const QString IndexFile::XML_PATIENT = "Patient";
const QString IndexFile::XML_PATIENT_NAME = "name";
const QString IndexFile::XML_SERIES = "Series";
const QString IndexFile::XML_SERIES_NAME = "name";
const QString IndexFile::XML_STUDY = "Study";
const QString IndexFile::XML_STUDY_NAME = "name";
const QString IndexFile::XML_DICOM_IMAGE = "DicomImage";
const QString IndexFile::XML_DICOM_IMAGE_FILE = "file";
const QString IndexFile::XML_DICOM_IMAGE_ZPOSITION = "zPosition";


IndexFile::IndexFile()
{
}



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
