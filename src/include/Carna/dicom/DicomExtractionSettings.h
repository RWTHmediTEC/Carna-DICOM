/*
 *  Copyright (C) 2010 - 2013 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#ifndef DICOMEXTRACTIONSETTINGS_H_6014714286
#define DICOMEXTRACTIONSETTINGS_H_6014714286

/** \file   DicomExtractionSettings.h
  * \brief  Defines \ref Carna::dicom::DicomExtractionSettings
  */

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// DicomExtractionSettings
// ----------------------------------------------------------------------------------

class DicomExtractionSettings
{

public:

    DicomExtractionSettings()
    {
    }

    DicomExtractionSettings( const QString& destination
                           , const std::set< const Series* >& series )
        : destination( destination )
        , series( series )
    {
    }


    QString destination;

    std::set< const Series* > series;

}; // DicomExtractionSettings



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // DICOMEXTRACTIONSETTINGS_H_6014714286
