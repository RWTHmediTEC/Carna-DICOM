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

#ifndef DICOMEXTRACTIONSETTINGS_H_3294808493
#define DICOMEXTRACTIONSETTINGS_H_3294808493

/** \file   DICOMExtractionSettings.h
  * \brief  Defines \ref Carna::dicom::DICOMExtractionSettings
  */

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// DICOMExtractionSettings
// ----------------------------------------------------------------------------------

class DICOMExtractionSettings
{

public:

    DICOMExtractionSettings()
    {
    }

    DICOMExtractionSettings( const QString& destination, const std::set< const Series* >& series )
        : destination( destination )
        , series( series )
    {
    }

    QString destination;
    std::set< const Series* > series;

}; // DICOMExtractionSettings



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // DICOMEXTRACTIONSETTINGS_H_3294808493
