cmake_minimum_required(VERSION 2.8.7)
project( Carna-DICOM )

set( MAJOR_VERSION		0 )
set( MINOR_VERSION		1 )
set( RELEASE_VERSION	0 )

############################################

set(CMAKE_DEBUG_POSTFIX d)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set( FULL_VERSION		${MAJOR_VERSION}.${MINOR_VERSION}.${RELEASE_VERSION} )
set( TARGET_NAME		${PROJECT_NAME}-${FULL_VERSION} )

############################################
# Options
# Set default options for this build
############################################

option(BUILD_DOC	"Build and install the API documentation"	OFF)
option(BUILD_TEST	"Build the unit tests"						OFF)

############################################
# Macro that sets variable to default value
# only when the variable isn't defined yet
############################################

macro( option_default_to var_name default_val var_type doc_string )
	if( NOT DEFINED ${var_name} )
		set(${var_name} ${default_val})
	endif()
	set(${var_name} ${${var_name}} CACHE ${var_type} ${doc_string} FORCE)
endmacro()

############################################
# Locate Find<ModuleName>.cmake scripts
############################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/misc/CMake-Modules)

############################################
# Run MediTEC compatiblity script
############################################

add_subdirectory(misc/MediTEC)

############################################
# Define default paths for the installation
############################################

# set default library and header destinations (relative to CMAKE_INSTALL_PREFIX)
option_default_to(INSTALL_LIBRARY_DIR "lib" String "Installation directory for libraries")
option_default_to(INSTALL_HEADERS_DIR "include" String "Installation directory for headers")

# set default destination for CMake modules
if( WIN32 )
	option_default_to(INSTALL_CMAKE_DIR "share/cmake-3.0/Modules" String "Installation directory for CMake files")
else( WIN32 )
	option_default_to(INSTALL_CMAKE_DIR "share/cmake/Modules" String "Installation directory for CMake files")
endif( WIN32 )

############################################
# Normalize installation paths
# (get rid of Windows-style delimiters)
############################################

file(	TO_CMAKE_PATH	${INSTALL_LIBRARY_DIR}	INSTALL_LIBRARY_DIR )
file(	TO_CMAKE_PATH	${INSTALL_HEADERS_DIR}	INSTALL_HEADERS_DIR )
file(	TO_CMAKE_PATH	${INSTALL_CMAKE_DIR}	INSTALL_CMAKE_DIR )

############################################

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/src/doc/Doxyfile.in
				${CMAKE_CURRENT_SOURCE_DIR}/src/doc/Doxyfile @ONLY)

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/misc/FindCarnaDICOM.cmake.in
				${CMAKE_CURRENT_BINARY_DIR}/misc/FindCarnaDICOM.cmake @ONLY)

############################################
# Find required dependencies
############################################

# Qt

set( QT_USE_QTMAIN		TRUE )
set( QT_USE_QTXML		TRUE )
find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

# GDCM

find_package( GDCM 2.0.16 REQUIRED )
include_directories( ${GDCM_INCLUDE_DIR} )

# Carna

find_package( Carna REQUIRED )
include_directories( ${CARNA_INCLUDE_DIR} )

# Project

include_directories(${CMAKE_PROJECT_DIR}include)
set( PUBLIC_QOBJECT_HEADERS
		include/Carna/dicom/DicomController.h
		include/Carna/dicom/DicomSceneFactory.h
)
set( PUBLIC_HEADERS
		${PUBLIC_QOBJECT_HEADERS}
		include/Carna/dicom/CarnaDICOM.h
		include/Carna/dicom/Series.h
		include/Carna/dicom/SeriesElement.h
		include/Carna/dicom/SeriesLoadingRequest.h
	)
include_directories(${CMAKE_PROJECT_DIR}src/include)
set( PRIVATE_QOBJECT_HEADERS
		src/include/Carna/dicom/DicomManager.h
		src/include/Carna/dicom/SeriesLoadingWorker.h
		src/include/Carna/dicom/DicomOpeningDialog.h
		src/include/Carna/dicom/QtDicomSeriesOpeningController.h
		src/include/Carna/dicom/SeriesPreview.h
		src/include/Carna/dicom/SeriesView.h
		src/include/Carna/dicom/ToggleSeriesPreview.h
		src/include/Carna/dicom/UInt16VolumeComputationWorker.h
)
set( PRIVATE_HEADERS
		${PRIVATE_QOBJECT_HEADERS}
		src/include/Carna/dicom/Patient.h
		src/include/Carna/dicom/Study.h
		src/include/dicom-interface/dicomImage.h
		src/include/dicom-interface/dicomSeries.h
	)
set( SRC
		src/dicom/DicomManager.cpp
		src/dicom/Patient.cpp
		src/dicom/Series.cpp
		src/dicom/SeriesElement.cpp
		src/dicom/SeriesLoadingWorker.cpp
		src/dicom/Study.cpp
		src/dicom/DicomController.cpp
		src/dicom/DicomOpeningDialog.cpp
		src/dicom/DicomSceneFactory.cpp
		src/dicom/QtDicomSeriesOpeningController.cpp
		src/dicom/SeriesPreview.cpp
		src/dicom/SeriesView.cpp
		src/dicom/ToggleSeriesPreview.cpp
		src/dicom/UInt16VolumeComputationWorker.cpp
		src/dicom-interface/dicomImage.cpp
		src/dicom-interface/dicomSeries.cpp
	)
set( FORMS
		""
	)
set( RESOURCES
	)
set( DOC_SRC
		src/doc/Doxyfile.in
		src/doc/doc_00style.css
		src/doc/index.dox
	)
	
QT4_WRAP_CPP( PUBLIC_HEADERS_MOC ${PUBLIC_QOBJECT_HEADERS} )
QT4_WRAP_CPP( PRIVATE_HEADERS_MOC ${PRIVATE_QOBJECT_HEADERS} )
QT4_WRAP_UI( FORMS_HEADERS ${FORMS} )
QT4_ADD_RESOURCES( RESOURCES_RCC ${RESOURCES} )
	
############################################
# Exclude all source files from build,
# since we are using unity build.
############################################
set_source_files_properties(	${SRC}
								${PUBLIC_HEADERS_MOC}
								${PRIVATE_HEADERS_MOC}
								${FORMS_HEADERS}
								${RESOURCES_RCC}
								${DOC_SRC}
	PROPERTIES
	HEADER_FILE_ONLY TRUE )
############################################
	
############################################
# Write unity build file
############################################
set( UNITY_BUILD_FILE	${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-unitybuild.cpp )

file( REMOVE	${UNITY_BUILD_FILE} )
file( WRITE		${UNITY_BUILD_FILE} "// This file is automatically generated by CMake.\n\n" )

foreach( SOURCE_FILE ${SRC} )
	file( APPEND	${UNITY_BUILD_FILE}	"#include \"${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}\"\n" )
endforeach( SOURCE_FILE )

foreach( QT_GENERATED_SOURCE_FILE ${PUBLIC_HEADERS_MOC} ${PRIVATE_HEADERS_MOC} ${FORMS_HEADERS} ${RESOURCES_RCC} )
	file( APPEND	${UNITY_BUILD_FILE}	"#include \"${QT_GENERATED_SOURCE_FILE}\"\n" )
endforeach( QT_GENERATED_SOURCE_FILE )
############################################
	
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_library( ${TARGET_NAME} SHARED
			${SRC}
			${UNITY_BUILD_FILE}
			${PUBLIC_HEADERS}
			${PRIVATE_HEADERS}
			${PUBLIC_HEADERS_MOC}
			${PRIVATE_HEADERS_MOC}
			${FORMS_HEADERS}
			${RESOURCES_RCC}
			${DOC_SRC}
		)

string( TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER )
string( REGEX REPLACE "-" "_" PROJECT_NAME_UPPER ${PROJECT_NAME_UPPER} )
add_definitions( -D${PROJECT_NAME_UPPER}_EXPORT -DNOMINMAX -D_WINDOWS -D_USRDLL -D_CRT_SECURE_NO_WARNINGS )


############################################
# Add dependencies to the linker
############################################

target_link_libraries( ${TARGET_NAME}
		ws2_32
		rpcrt4
		${QT_LIBRARIES}
		${GDCM_LIBRARIES}
		${CARNA_LIBRARIES}
	)






#vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
##vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
### NO CHANGES EVER NECESSARY BELOW THIS POINT!!
##vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
#vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

string( REGEX REPLACE "\\\\" "/" INSTALL_DIR $ENV{MEDITEC_LIBS}/${PROJECT_NAME}/${FULL_VERSION} )

install(	TARGETS	${TARGET_NAME}
		RUNTIME	DESTINATION	$ENV{MEDITEC_LIBS}/bin
		)
install(	TARGETS	${TARGET_NAME}
		RUNTIME	DESTINATION	${INSTALL_DIR}/bin
		ARCHIVE	DESTINATION	${INSTALL_DIR}/bin
		LIBRARY	DESTINATION	${INSTALL_DIR}/bin
		)
install(	FILES	${CMAKE_CURRENT_BINARY_DIR}/misc/configure.cmake ${CMAKE_CURRENT_BINARY_DIR}/misc/install.bat
		DESTINATION 	${INSTALL_DIR}
		)
install(	DIRECTORY	${CMAKE_PROJECT_DIR}include
		DESTINATION 	${INSTALL_DIR}
		)
if (WIN32)
	install(	FILES	${CMAKE_CURRENT_BINARY_DIR}/bin/${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}.pdb
			DESTINATION		$ENV{MEDITEC_LIBS}/bin
			OPTIONAL
			)
	install(	FILES	${CMAKE_CURRENT_BINARY_DIR}/bin/${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}.pdb
			DESTINATION		${INSTALL_DIR}/bin
			OPTIONAL
			)
endif()


############################################
# Unit tests
############################################
if( BUILD_TEST )
#	add_subdirectory( test )
endif()
############################################

############################################
# Doxygen API documentation
############################################
if( BUILD_DOC )
	find_package( Doxygen REQUIRED )
	if( DOXYGEN_FOUND )
		add_custom_target(	doc ALL
						${DOXYGEN_EXECUTABLE} 
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/doc
						COMMENT "Generating API documentation with Doxygen" VERBATIM
					)
		install(	DIRECTORY 	${CMAKE_CURRENT_SOURCE_DIR}/doc
				DESTINATION		${INSTALL_DIR}
			)
	endif( DOXYGEN_FOUND )
endif()
############################################
