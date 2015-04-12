cmake_minimum_required(VERSION 2.8.7)

############################################
# It is important to use the root binary directory,
# so that the proper LIB from the tested project is found
# and the EXE is placed in the same directory as the DLL.
############################################
set( CMAKE_CURRENT_BINARY_DIR ${CMAKE_BINARY_DIR} )
############################################

set( CMAKE_DEBUG_POSTFIX d )
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )



############################################
# Project name
############################################
project( CarnaDICOMDemo )
set( TARGET_NAME	${PROJECT_NAME}-${FULL_VERSION} )
############################################



############################################
# Find dependencies and add them later to the linker (see below)
############################################

# Qt
find_package( Qt4 4.8.0 COMPONENTS QtCore QtGui )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

# Eigen
find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )

# Carna
find_package( Carna 3.0.1 REQUIRED )
include_directories( ${CARNA_INCLUDE_DIR} )

# CarnaQt
find_package( CarnaQt 1.0.1 )
include_directories( ${CARNAQT_INCLUDE_DIR} )

# CarnaDICOM
include_directories( ${CMAKE_SOURCE_DIR}/include )
link_directories( ${CMAKE_BINARY_DIR}/bin )

############################################
# Project
############################################

include_directories(${CMAKE_PROJECT_DIR}src)
include_directories(${CMAKE_PROJECT_DIR}../Tools)
set( HEADERS
		""
	)
set( QOBJECT_HEADERS
		""
	)
set( SRC
		src/main.cpp
	)
set( FORMS
		""
	)
set( RESOURCES
		""
	)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

if( QT_FOUND AND CARNAQT_FOUND )
	
	unset( RESOURCES_RCC )
	QT4_WRAP_CPP( QOBJECT_HEADERS_MOC ${QOBJECT_HEADERS} )
	QT4_WRAP_UI( FORMS_HEADERS ${FORMS} )
	QT4_ADD_RESOURCES( RESOURCES_RCC ${RESOURCES} )

	add_definitions(
			-DNOMINMAX
			-D_SCL_SECURE_NO_WARNINGS
		)
	remove_definitions( -DCARNADICOM_EXPORT )
	
	add_executable( ${TARGET_NAME} WIN32
			${SRC}
			${HEADERS}
			${QOBJECT_HEADERS}
			${QOBJECT_HEADERS_MOC}
			${FORMS_HEADERS}
			${RESOURCES_RCC}
		)

	include( "../../misc/compiler_specific.cmake" )

	target_link_libraries( ${TARGET_NAME}
			${QT_LIBRARIES}
			${CARNA_LIBRARIES}
			${CARNAQT_LIBRARIES}
			optimized	CarnaDICOM-${FULL_VERSION}
			debug		CarnaDICOM-${FULL_VERSION}${CMAKE_DEBUG_POSTFIX}
		)
	
	if( MSVC )
		set_target_properties(
				${TARGET_NAME} PROPERTIES
				WIN32_EXECUTABLE YES
				LINK_FLAGS "/ENTRY:mainCRTStartup"
			)
	endif( MSVC )

else( QT_FOUND AND CARNAQT_FOUND )

	message(WARNING
		"\nQt or CarnaQt not found, skipping ${PROJECT_NAME}.\n"
		  "You don't have to worry about this unless you need "
		  "an executable demo project within your CarnaDICOM "
		  "development environment, i.e. when you are going to "
		  "work on CarnaDICOM.\n")

endif( QT_FOUND AND CARNAQT_FOUND )
