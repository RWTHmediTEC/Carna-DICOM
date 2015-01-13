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

#ifndef CARNA_DICOM_H_6014714286
#define CARNA_DICOM_H_6014714286

#include <QtCore/qglobal.h>

#ifdef CARNADICOM_EXPORT
#   define CARNADICOM_LIB Q_DECL_EXPORT
#else
#   define CARNADICOM_LIB Q_DECL_IMPORT
#endif

namespace Carna
{
    namespace dicom
    {
        class DicomSeries;
        class DicomImage;
        class DicomController;
        class DicomSceneFactory;
        class Series;
        class SeriesView;
        class SeriesLoadingRequest;
    }
}

#endif // CARNA_DICOM_H_6014714286
