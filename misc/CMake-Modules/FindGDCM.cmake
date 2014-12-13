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
# Leonid Kostrykin, 13.12.2014
############################################

find_path(	GDCM_INCLUDE_DIR
			gdcm/gdcmReader.h gdcm/gdcmImage.h
			PATHS ENV MEDITEC_LIBS
			PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/include"
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
find_package_handle_standard_args(GDCM
	REQUIRED_VARS GDCM_INCLUDE_DIR
	GDCM_LIBRARY_COMMON_RELEASE GDCM_LIBRARY_COMMON_DEBUG
	GDCM_LIBRARY_DSED_RELEASE GDCM_LIBRARY_DSED_DEBUG
	GDCM_LIBRARY_MSFF_RELEASE GDCM_LIBRARY_MSFF_DEBUG
	GDCM_LIBRARY_EXPAT_RELEASE GDCM_LIBRARY_EXPAT_DEBUG
	GDCM_LIBRARY_JPEG12_RELEASE GDCM_LIBRARY_JPEG12_DEBUG
	GDCM_LIBRARY_JPEG8_RELEASE GDCM_LIBRARY_JPEG8_DEBUG
	GDCM_LIBRARY_ZLIB_RELEASE GDCM_LIBRARY_ZLIB_DEBUG
	GDCM_LIBRARY_DICT_RELEASE GDCM_LIBRARY_DICT_DEBUG
	GDCM_LIBRARY_IOD_RELEASE GDCM_LIBRARY_IOD_DEBUG
	GDCM_LIBRARY_CHARLS_RELEASE GDCM_LIBRARY_CHARLS_DEBUG
	GDCM_LIBRARY_GETOPT_RELEASE GDCM_LIBRARY_GETOPT_DEBUG
	GDCM_LIBRARY_JPEG16_RELEASE GDCM_LIBRARY_JPEG16_DEBUG
	GDCM_LIBRARY_OPENJPEG_RELEASE GDCM_LIBRARY_OPENJPEG_DEBUG)

# set paths to library files
if( GDCM_FOUND )
	set( GDCM_LIBRARIES
			optimized	${GDCM_LIBRARY_COMMON_RELEASE}		debug	${GDCM_LIBRARY_COMMON_DEBUG}
			optimized	${GDCM_LIBRARY_DSED_RELEASE}		debug	${GDCM_LIBRARY_DSED_DEBUG}
			optimized	${GDCM_LIBRARY_MSFF_RELEASE}		debug	${GDCM_LIBRARY_MSFF_DEBUG}
			optimized	${GDCM_LIBRARY_EXPAT_RELEASE}		debug	${GDCM_LIBRARY_EXPAT_DEBUG}
			optimized	${GDCM_LIBRARY_JPEG12_RELEASE}		debug	${GDCM_LIBRARY_JPEG12_DEBUG}
			optimized	${GDCM_LIBRARY_JPEG8_RELEASE}		debug	${GDCM_LIBRARY_JPEG8_DEBUG}
			optimized	${GDCM_LIBRARY_ZLIB_RELEASE}		debug	${GDCM_LIBRARY_ZLIB_DEBUG}
			optimized	${GDCM_LIBRARY_DICT_RELEASE}		debug	${GDCM_LIBRARY_DICT_DEBUG}
			optimized	${GDCM_LIBRARY_IOD_RELEASE}			debug	${GDCM_LIBRARY_IOD_DEBUG}
			optimized	${GDCM_LIBRARY_CHARLS_RELEASE}		debug	${GDCM_LIBRARY_CHARLS_DEBUG}
			optimized	${GDCM_LIBRARY_GETOPT_RELEASE}		debug	${GDCM_LIBRARY_GETOPT_DEBUG}
			optimized	${GDCM_LIBRARY_JPEG16_RELEASE}		debug	${GDCM_LIBRARY_JPEG16_DEBUG}
			optimized	${GDCM_LIBRARY_OPENJPEG_RELEASE}	debug	${GDCM_LIBRARY_OPENJPEG_DEBUG} )
endif()