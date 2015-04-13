############################################
# Locates GDCM library.
# ------------------------------------------
# If GDCM headers and library files are not
# found in the standard CMake locations,
# then %MEDITEC_LIBS%/gdcm/<version> is
# checked. In this, letter case it is
# necessary that the version is matched
# exactly. Consider using symbolic links
# when you have a compatible version.
#-------------------------------------------
# The following variables are set:
#	GDCM_INCLUDE_DIR	points to headers
#	GDCM_LIBRARIES		points to lib files
#	GDCM_FOUND			indicates success
#-------------------------------------------
# Leonid Kostrykin, 13.4.2015
############################################

find_path(	GDCM_INCLUDE_DIR
		    gdcmReader.h gdcmImage.h
		    PATHS "${CMAKE_INSTALL_PREFIX}/include" ENV MEDITEC_LIBS
		    PATH_SUFFIXES "gdcm-2.0" "gdcm/${GDCM_FIND_VERSION}/include/gdcm"
		    DOC "GDCM headers")
			
############################################
# Find RELEASE libraries
############################################

find_library(	GDCM_LIBRARY_COMMON_RELEASE
				NAMES "gdcmCommon-${GDCM_FIND_VERSION}" gdcmCommon
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-common library (release)")

find_library(	GDCM_LIBRARY_DSED_RELEASE
				NAMES "gdcmDSED-${GDCM_FIND_VERSION}" gdcmDSED
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-dsed library (release)")

find_library(	GDCM_LIBRARY_MSFF_RELEASE
				NAMES "gdcmMSFF-${GDCM_FIND_VERSION}" gdcmMSFF
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-msff library (release)")

find_library(	GDCM_LIBRARY_EXPAT_RELEASE
				NAMES "gdcmexpat-${GDCM_FIND_VERSION}" gdcmexpat
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-expat library (release)")

find_library(	GDCM_LIBRARY_JPEG12_RELEASE
				NAMES "gdcmjpeg12-${GDCM_FIND_VERSION}" gdcmjpeg12
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-jpeg12 library (release)")

find_library(	GDCM_LIBRARY_JPEG8_RELEASE
				NAMES "gdcmjpeg8-${GDCM_FIND_VERSION}" gdcmjpeg8
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-jpeg8 library (release)")

find_library(	GDCM_LIBRARY_ZLIB_RELEASE
				NAMES "gdcmzlib-${GDCM_FIND_VERSION}" gdcmzlib
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-zlib library (release)")

find_library(	GDCM_LIBRARY_DICT_RELEASE
				NAMES "gdcmDICT-${GDCM_FIND_VERSION}" gdcmDICT
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-dict library (release)")

find_library(	GDCM_LIBRARY_IOD_RELEASE
				NAMES "gdcmIOD-${GDCM_FIND_VERSION}" gdcmIOD
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-iod library (release)")

find_library(	GDCM_LIBRARY_CHARLS_RELEASE
				NAMES "gdcmcharls-${GDCM_FIND_VERSION}" gdcmcharls
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-charls library (release)")

find_library(	GDCM_LIBRARY_GETOPT_RELEASE
				NAMES "gdcmgetopt-${GDCM_FIND_VERSION}" gdcmgetopt
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-getopt library (release)")

find_library(	GDCM_LIBRARY_JPEG16_RELEASE
				NAMES "gdcmjpeg16-${GDCM_FIND_VERSION}" gdcmjpeg16
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-jpeg16 library (release)")

find_library(	GDCM_LIBRARY_OPENJPEG_RELEASE
				NAMES "gdcmopenjpeg-${GDCM_FIND_VERSION}" gdcmopenjpeg
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-openjpeg library (release)")
			
############################################
# Find DEBUG libraries
############################################

find_library(	GDCM_LIBRARY_COMMON_DEBUG
				NAMES "gdcmCommon-${GDCM_FIND_VERSION}d" gdcmCommond
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-common library (debug)")

find_library(	GDCM_LIBRARY_DSED_DEBUG
				NAMES "gdcmDSED-${GDCM_FIND_VERSION}d" gdcmDSEDd
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-dsed library (debug)")

find_library(	GDCM_LIBRARY_MSFF_DEBUG
				NAMES "gdcmMSFF-${GDCM_FIND_VERSION}d" gdcmMSFFd
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-msff library (debug)")

find_library(	GDCM_LIBRARY_EXPAT_DEBUG
				NAMES "gdcmexpat-${GDCM_FIND_VERSION}d" gdcmexpatd
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-expat library (debug)")

find_library(	GDCM_LIBRARY_JPEG12_DEBUG
				NAMES "gdcmjpeg12-${GDCM_FIND_VERSION}d" gdcmjpeg12d
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-jpeg12 library (debug)")

find_library(	GDCM_LIBRARY_JPEG8_DEBUG
				NAMES "gdcmjpeg8-${GDCM_FIND_VERSION}d" gdcmjpeg8d
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-jpeg8 library (debug)")

find_library(	GDCM_LIBRARY_ZLIB_DEBUG
				NAMES "gdcmzlib-${GDCM_FIND_VERSION}d" gdcmzlibd
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-zlib library (debug)")

find_library(	GDCM_LIBRARY_DICT_DEBUG
				NAMES "gdcmDICT-${GDCM_FIND_VERSION}d" gdcmDICTd
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-dict library (debug)")

find_library(	GDCM_LIBRARY_IOD_DEBUG
				NAMES "gdcmIOD-${GDCM_FIND_VERSION}d" gdcmIODd
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-iod library (debug)")

find_library(	GDCM_LIBRARY_CHARLS_DEBUG
				NAMES "gdcmcharls-${GDCM_FIND_VERSION}d" gdcmcharlsd
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-charls library (debug)")

find_library(	GDCM_LIBRARY_GETOPT_DEBUG
				NAMES "gdcmgetopt-${GDCM_FIND_VERSION}d" gdcmgetoptd
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-getopt library (debug)")

find_library(	GDCM_LIBRARY_JPEG16_DEBUG
				NAMES "gdcmjpeg16-${GDCM_FIND_VERSION}d" gdcmjpeg16d
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-jpeg16 library (debug)")

find_library(	GDCM_LIBRARY_OPENJPEG_DEBUG
				NAMES "gdcmopenjpeg-${GDCM_FIND_VERSION}d" gdcmopenjpegd
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM-openjpeg library (debug)")

# handles REQUIRED, QUIET and version-related
# arguments and also sets the _FOUND variable
find_package_handle_standard_args( GDCM
	REQUIRED_VARS GDCM_INCLUDE_DIR
	GDCM_LIBRARY_COMMON_RELEASE
	GDCM_LIBRARY_DSED_RELEASE
	GDCM_LIBRARY_MSFF_RELEASE
	GDCM_LIBRARY_JPEG12_RELEASE
	GDCM_LIBRARY_JPEG8_RELEASE
	GDCM_LIBRARY_DICT_RELEASE
	GDCM_LIBRARY_IOD_RELEASE
	GDCM_LIBRARY_JPEG16_RELEASE )

# set paths to library files
if( GDCM_FOUND )

	set( GDCM_LIBRARIES "" )

    # set-up additional dependencies on Windows systems
    if( WIN32 )
        list( APPEND GDCM_LIBRARIES Ws2_32 rpcrt4 )
    endif()
			
	macro( add_gdcm_library lib )
	    if( ${lib}_DEBUG )
    	    list( APPEND GDCM_LIBRARIES optimized ${${lib}_RELEASE} debug ${${lib}_DEBUG} )
	    else()
	        list( APPEND GDCM_LIBRARIES ${${lib}_RELEASE} )
	    endif()
	endmacro()
	
	add_gdcm_library( GDCM_LIBRARY_COMMON )
	add_gdcm_library( GDCM_LIBRARY_DSED )
	add_gdcm_library( GDCM_LIBRARY_MSFF )
	add_gdcm_library( GDCM_LIBRARY_JPEG12 )
	add_gdcm_library( GDCM_LIBRARY_JPEG8 )
	add_gdcm_library( GDCM_LIBRARY_DICT )
	add_gdcm_library( GDCM_LIBRARY_IOD )
	add_gdcm_library( GDCM_LIBRARY_JPEG16 )
	
	macro( add_gdcm_library_optional lib )
	    if( ${lib}_RELEASE )
	        add_gdcm_library( ${lib} )
	    endif()
	endmacro()
	
	add_gdcm_library_optional( GDCM_LIBRARY_EXPAT )
	add_gdcm_library_optional( GDCM_LIBRARY_ZLIB )
	add_gdcm_library_optional( GDCM_LIBRARY_CHARLS )
	add_gdcm_library_optional( GDCM_LIBRARY_GETOPT )
	add_gdcm_library_optional( GDCM_LIBRARY_OPENJPEG )
	
endif()
