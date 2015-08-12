#---------------------------------------------------#
#          CMake Project Configuration              #
#---------------------------------------------------#
# CMake will issue a warning if you don't set this
cmake_minimum_required( VERSION 2.8 )

# If you don't name the project, it will take the default name "Project"
# Note: this must be set in the beginning of the CMakeLists file
project( labirinth )

# Add the extra cmake module script path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#---------------------------------------------------#
#              OpenGL Configuration                 #
#---------------------------------------------------#
# Automatically find GL libraries
FIND_PACKAGE( OpenGL )
FIND_PACKAGE( GLEW REQUIRED )
FIND_PACKAGE( GLFW REQUIRED )
FIND_PACKAGE( GLM REQUIRED )
FIND_PACKAGE ( X11 REQUIRED )


IF ( X11_FOUND )
      INCLUDE_DIRECTORIES ( ${X11_INCLUDE_DIR} )
      LINK_LIBRARIES ( ${X11_LIBRARIES} )
      MESSAGE ( STATUS " X11_INCLUDE_DIR: " ${X11_INCLUDE_DIR} )
      MESSAGE ( STATUS " X11_LIBRARIES: " ${X11_LIBRARIES} )
ENDIF ( X11_FOUND )

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
    ../gre/timer.h
)

set( FX_CLASSES 
        ../greFX/explosion.cpp
        ../greFX/explosion.hpp
)

set( TESTING_BASE_APP basic/main.cpp )

set( TESTING_BASE_64k_APP basic_64k/main.cpp )

set( TESTING_BASE_ORTHO_APP basicOrtho/main.cpp )

set( TESTING_ORTHO_PANEL_APP orthoPanel/main.cpp )

set( MULTY_SCENE_APP sceneSwitcher/main.cpp )

set( EXPLOSION_SCENE_APP explosionScene/main.cpp )

#---------------------------------------------------#
#               Project Configuration               #
#---------------------------------------------------#
# Add all sources defined above to the program
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR} ${GLM_INCLUDE_DIR} )
link_directories( ${CMAKE_BINARY_DIR}/bin ${GLFW_LIBRARY_DIRS})

# Add _d to the library in debug
set( CMAKE_DEBUG_POSTFIX _d )

add_executable( basicCubeMoving ${LAB_HEADERS} ${LAB_SRCS} ${TESTING_BASE_APP} )
target_link_libraries( basicCubeMoving ${GLEW_LIBRARY} ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} )

add_executable( basicCubeMoving_64k  ${TESTING_BASE_64k_APP} )
target_link_libraries( basicCubeMoving_64k ${OPENGL_gl_LIBRARY} )

add_executable( basicOrtho ${LAB_HEADERS} ${LAB_SRCS} ${TESTING_BASE_ORTHO_APP} )
target_link_libraries( basicOrtho ${GLEW_LIBRARY} ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} )

add_executable( basicOrthoPanel ${LAB_HEADERS} ${LAB_SRCS} ${TESTING_ORTHO_PANEL_APP} )
target_link_libraries( basicOrthoPanel ${GLEW_LIBRARY} ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} )

add_executable( multyScene ${LAB_HEADERS} ${LAB_SRCS} ${MULTY_SCENE_APP} )
target_link_libraries( multyScene ${GLEW_LIBRARY} ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} )

add_executable( explosionScene ${LAB_HEADERS} ${LAB_SRCS} ${EXPLOSION_SCENE_APP} ${FX_CLASSES} )
target_link_libraries( explosionScene ${GLEW_LIBRARY} ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} )



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
