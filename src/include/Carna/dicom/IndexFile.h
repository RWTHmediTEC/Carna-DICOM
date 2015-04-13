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

#ifndef INDEXFILE_H_3294808493
#define INDEXFILE_H_3294808493
#if !CARNAQT_DISABLED

/** \file   IndexFile.h
  * \brief  Defines \ref Carna::dicom::IndexFile.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// IndexFile
// ----------------------------------------------------------------------------------

class IndexFile
{

    NON_COPYABLE

    IndexFile();

public:

    const static QString XML_DICOM_INDEX;
    const static QString XML_PATIENT;
    const static QString XML_PATIENT_NAME;
    const static QString XML_SERIES;
    const static QString XML_SERIES_NAME;
    const static QString XML_STUDY;
    const static QString XML_STUDY_NAME;
    const static QString XML_DICOM_IMAGE;
    const static QString XML_DICOM_IMAGE_FILE;
    const static QString XML_DICOM_IMAGE_ZPOSITION;

}; // IndexFile



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
#endif // INDEXFILE_H_3294808493
