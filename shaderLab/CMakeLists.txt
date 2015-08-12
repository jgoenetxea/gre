#---------------------------------------------------#
#          CMake Project Configuration              #
#---------------------------------------------------#
# CMake will issue a warning if you don't set this
cmake_minimum_required( VERSION 2.8 )

# If you don't name the project, it will take the default name "Project"
# Note: this must be set in the beginning of the CMakeLists file
project( shaderLab )

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Add the extra cmake module script path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#---------------------------------------------------#
#              OpenGL Configuration                 #
#---------------------------------------------------#
# Automatically find GL libraries
FIND_PACKAGE( GLM REQUIRED )
FIND_PACKAGE( OpenGL )

#---------------------------------------------------#
#              Project Files                        #
#---------------------------------------------------#
# List source files in a variable so we can use it later more conveniently
set( LAB_SRCS
    ../gre/camera.cpp
    ../gre/projectiveCamera.cpp
    ../gre/orthoCamera.cpp
    ../gre/node.cpp
    ../gre/scene.cpp
    ../gre/obj.cpp
    ../gre/renderer.cpp
    ../gre/translation.cpp
    ../gre/objloader.cpp
    ../gre/shaderProgram.cpp
    ../gre/shapes.cpp
    ../gre/texture.cpp
)

set( LAB_HEADERS
    ../gre/camera.hpp
    ../gre/projectiveCamera.hpp
    ../gre/orthoCamera.hpp
    ../gre/node.hpp
    ../gre/scene.hpp
    ../gre/obj.hpp
    ../gre/renderer.hpp
    ../gre/translation.hpp
    ../gre/objloader.hpp
    ../gre/shaderProgram.hpp
    ../gre/shapes.hpp
    ../gre/texture.hpp
)

set( FX_CLASSES 
    ../greFX/explosion.cpp
    ../greFX/explosion.hpp
)

set( TESTING_BASE_APP
                main.cpp
	)
set( TESTING_BASE_APP_SOURCE
                mainwindow.cpp
                glView.cpp
	)
set( TESTING_BASE_APP_HEADER
                mainwindow.h
                glView.h
                testGlView.h
	)

SET( SCENE_DEFINITION
                scenes/movingCube.cpp
                scenes/movingCube.h
                scenes/simpleQuad.cpp
                scenes/simpleQuad.h
    )
	
SET( PROJECT_FORMS
	forms/mainwindow.ui
	)
SET( PROJECT_RESOURCES resources/images.qrc )

#---------------------------------------------------#
#              Qt5 Configuration                 	#
#---------------------------------------------------#
FIND_PACKAGE( Qt5Widgets REQUIRED )
FIND_PACKAGE( Qt5OpenGL REQUIRED )
SET( QT_USE_QTOPENGL TRUE )

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

QT5_WRAP_CPP( PROJECT_HEADERS_MOC ${TESTING_BASE_APP_HEADER} )
QT5_WRAP_UI( PROJECT_FORMS_HEADERS ${PROJECT_FORMS} )
QT5_ADD_RESOURCES( PROJECT_RESOURCES_RCC ${PROJECT_RESOURCES} )
SET( QT_INCLUDE_DIR ${Qt5Widgets_INCLUDE_DIRS} )

#SOURCE_GROUP( "Generated Files" FILES
#	${PROJECT_HEADERS_MOC}
#	${PROJECT_FORMS_HEADERS}
#	${PROJECT_RESOURCES_RCC}
#	)

#---------------------------------------------------#
#               Project Configuration               #
#---------------------------------------------------#
#SOURCE_GROUP( "Project Files" FILES
#	${TESTING_BASE_APP_SOURCE}
#	${TESTING_BASE_APP_HEADER}
#	)
	
# Add all sources defined above to the program
INCLUDE_DIRECTORIES(
                                        ${CMAKE_CURRENT_SOURCE_DIR}
                                        ${CMAKE_CURRENT_SOURCE_DIR}/include
                                        ${CMAKE_BINARY_DIR}
                                        ${GLM_INCLUDE_DIR}
					${QT_INCLUDE_DIR} 
					)


# Add _d to the library in debug
SET( CMAKE_DEBUG_POSTFIX _d )

ADD_EXECUTABLE( shaderLab
                                ${LAB_HEADERS}
                                ${LAB_SRCS}
                                ${TESTING_BASE_APP}
                                ${SCENE_DEFINITION}
                                ${TESTING_BASE_APP_SOURCE}
                                ${TESTING_BASE_APP_HEADER}
                                ${PROJECT_FORMS_HEADERS}
                                ${PROJECT_RESOURCES_RCC}
)

MESSAGE("GLEW libraries: ${GLEW_LIBRARY}")
TARGET_LINK_LIBRARIES( shaderLab
    ${OPENGL_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5OpenGL_LIBRARIES}
)


#---------------------------------------------------#
#               Project Documentation               #
#---------------------------------------------------#
#option( BUILD_LABIRINTH_DOC "Build 'Labirinth' documentation" OFF )
#if( BUILD_LABIRINTH_DOC )
#	FIND_PACKAGE( Doxygen )
#	if( NOT DOXYGEN_FOUND )
#		message( FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly" )
#	endif()
#	
#	#-- Configure the Template Doxyfile for our specific project
#	configure_file( doc/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE )
#	#-- Add a custom target to run Doxygen when ever the project is built
#	add_custom_target( Docs ALL
#		COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
#		SOURCES ${PROJECT_BINARY_DIR}/Doxyfile )
# # IF you do NOT want the documentation to be generated EVERY time you build the project
#  # then leave out the 'ALL' keyword from the above command.
#endif()
