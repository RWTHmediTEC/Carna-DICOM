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

#ifndef CARNADICOM_H_3294808493
#define CARNADICOM_H_3294808493

#if defined( CARNADICOM_EXPORT )
#   if defined( _MSC_VER )
#       define CARNADICOM_LIB __declspec( dllexport )
#       pragma warning( disable : 4251 )
#   elif defined( _GCC )
#       define CARNADICOM_LIB __attribute__( ( visibility( "default" ) ) )
#   else
#       define CARNADICOM_LIB
#   endif
#else
#   if defined( _MSC_VER )
#       define CARNADICOM_LIB __declspec( dllimport )
#   else
#       define CARNADICOM_LIB
#   endif
#endif
#if defined( NO_OVERRIDE_KEYWORD )
#   define override
#endif

#include <Carna/Carna.h>

namespace Carna
{
    namespace dicom
    {
        class VolumeGridFactoryBase;
        class Series;
        
        template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType = void > class VolumeFactoryLoader;
    }
}

#endif // CARNADICOM_H_3294808493
