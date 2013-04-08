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
find_package( GLEW REQUIRED )
find_package( GLFW REQUIRED )
find_package( GLM REQUIRED )

#---------------------------------------------------#
#              Project Files                        #
#---------------------------------------------------#
# List source files in a variable so we can use it later more conveniently
set( LAB_SRCS
	main.cpp
	common/controls.cpp
	common/shader.cpp
	common/shapes.cpp
	common/texture.cpp
)

set( LAB_HEADERS
	common/controls.hpp
	common/shader.hpp
	common/shapes.hpp
	common/texture.hpp
)

#---------------------------------------------------#
#               Project Configuration               #
#---------------------------------------------------#
# Add all sources defined above to the program
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR} )
link_directories( ${CMAKE_BINARY_DIR}/bin ${GLFW_LIBRARIES})

# Add _d to the library in debug
set( CMAKE_DEBUG_POSTFIX _d )

add_executable( labirinth ${LAB_HEADERS} ${LAB_SRCS} )

target_link_libraries( labirinth ${GLEW_LIBS} ${GLFW_LIBS} )
#${GLFW_LIBRARIES} ${GLEW_LIBRARY}

#---------------------------------------------------#
#               Project Documentation               #
#---------------------------------------------------#
option( BUILD_LABIRINTH_DOC "Build 'Labirinth' documentation" OFF )
if( BUILD_LABIRINTH_DOC )
	FIND_PACKAGE( Doxygen )
	if( NOT DOXYGEN_FOUND )
		message( FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly" )
	endif()
	
	#-- Configure the Template Doxyfile for our specific project
	configure_file( doc/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE )
	#-- Add a custom target to run Doxygen when ever the project is built
	add_custom_target( Docs ALL
		COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
		SOURCES ${PROJECT_BINARY_DIR}/Doxyfile )
  # IF you do NOT want the documentation to be generated EVERY time you build the project
  # then leave out the 'ALL' keyword from the above command.
endif()