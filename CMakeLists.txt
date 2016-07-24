# TOP Level Configuration
# ------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.6.3)


# Project name & Configuration & Version
# ------------------------------------------------------------------------------------------------------
SET(PROJ_MAIN_NAME "GRE")
PROJECT(${PROJ_MAIN_NAME})

SET( ${PROJ_MAIN_NAME}_MAJOR_VERSION 0 CACHE STRING "Major version" FORCE)
SET( ${PROJ_MAIN_NAME}_MINOR_VERSION 1 CACHE STRING "Minor version" FORCE)
SET( ${PROJ_MAIN_NAME}_PATCH_VERSION 0 CACHE STRING "Patch version" FORCE)

# Include Cmake helpers
# ------------------------------------------------------------------------------------------------------
INCLUDE( cmake/scripts.cmake)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Configure variables
# ------------------------------------------------------------------------------------------------------

# Our custom Paths
SET(${PROJ_MAIN_NAME}_PATH_MAIN 	${CMAKE_SOURCE_DIR} 					          CACHE PATH "This directory contains initial Path")
SET(${PROJ_MAIN_NAME}_PATH_LIBS 	${PROJECT_BINARY_DIR}/bin				        CACHE PATH "This directory contains all libs")
SET(${PROJ_MAIN_NAME}_PATH_EXE 		${PROJECT_BINARY_DIR}/bin				        CACHE PATH "This directory contains executables")
SET(${PROJ_MAIN_NAME}_PATH_3RDPARTY 	${CMAKE_SOURCE_DIR}/3rdParty 				CACHE PATH "This directory contains 3rdparty libraries")
SET(${PROJ_MAIN_NAME}_PATH_INSTALL 	${PROJECT_BINARY_DIR}/install/${PROJ_MAIN_NAME} 	CACHE PATH "This directory to install prebuilt")

# Build type
IF(TARGET_PLATFORM STREQUAL TARGET_ANDROID)
  SET(${PROJ_MAIN_NAME}_LIB_TYPE "STATIC" CACHE STRING "Choose static or shared")
ELSEIF(TARGET_PLATFORM STREQUAL TARGET_WIN32)
  SET(${PROJ_MAIN_NAME}_LIB_TYPE "SHARED" CACHE STRING "Choose static or shared")
ELSEIF(TARGET_PLATFORM STREQUAL TARGET_LINUX)
  SET(${PROJ_MAIN_NAME}_LIB_TYPE "SHARED" CACHE STRING "Choose static or shared")
ELSEIF(TARGET_PLATFORM STREQUAL TARGET_APPLE)
  SET(${PROJ_MAIN_NAME}_LIB_TYPE "STATIC" CACHE STRING "Choose static or shared")
ELSEIF(TARGET_PLATFORM STREQUAL TARGET_IOS)
  SET(${PROJ_MAIN_NAME}_LIB_TYPE "STATIC" CACHE STRING "Choose static or shared")
ELSE()
   MESSAGE(FATAL_ERROR "ERROR ${PROJ_MAIN_NAME}_LIB_TYPE")
ENDIF()

# Configure options
# ------------------------------------------------------------------------------------------------------
OPTION(${PROJ_MAIN_NAME}_BUILD_SAMPLES       "Build samples"            					               ON )
OPTION(${PROJ_MAIN_NAME}_BUILD_TEST          "Build tests"              					               OFF )
OPTION(${PROJ_MAIN_NAME}_BUILD_DOCS          "Create build rules for Doxygen Documentation" 		 OFF )
OPTION(${PROJ_MAIN_NAME}_BUILD_PROFILE       "Add flags to profiling UNIX" 					             OFF )
OPTION(${PROJ_MAIN_NAME}_BUILD_CPP11         "Force compile with standard C++11" 				         ON )

# Configure CMake options
# ------------------------------------------------------------------------------------------------------
OPTION(${PROJ_MAIN_NAME}_DEPENDENCY_XXX     "Dependency sample" 				                      OFF )

# Configuration current project with cmake variables
# ------------------------------------------------------------------------------------------------------
OPTION(CMAKE_VERBOSE "Verbose mode" OFF )
SET(CMAKE_DEBUG_POSTFIX)
SET(CMAKE_INSTALL_PREFIX ${${PROJ_MAIN_NAME}_PATH_INSTALL})
SET(CMAKE_BINARY_DIR ${${PROJ_MAIN_NAME}_PATH_EXE})

# Dependencies
# ------------------------------------------------------------------------------------------------------
MESSAGE(" Automatically find GL libraries...")

# Automatically find GL libraries
#FIND_PACKAGE( OpenGL )
#FIND_PACKAGE( GLEW REQUIRED )
#FIND_PACKAGE( GLFW REQUIRED )
#FIND_PACKAGE( GLM REQUIRED )
#FIND_PACKAGE( X11 REQUIRED )

# Opengl
FIND_PACKAGE( OpenGL )

# GLM
MESSAGE("Looking for GLM_DIR...")
FIND_PACKAGE( GLM REQUIRED )

# GLFW make install in system
#IF(${PROJ_MAIN_NAME}_DEPENDENCY_GLFW)
#  MESSAGE("Adding GLFW")
#  SET(${PROJ_MAIN_NAME}_DEPENDENCY_GLFW_DIR NOT_FOUND CACHE PATH "The path where GLFW is placed")
#  IF(${PROJ_MAIN_NAME}_DEPENDENCY_GLFW_DIR STREQUAL NOT_FOUND)
#  	MESSAGE(FATAL_ERROR "Select GLFW Dir manually!")
#  ENDIF()
#ENDIF()

# Project Logic
# ------------------------------------------------------------------------------------------------------
IF(NOT TARGET_PLATFORM STREQUAL TARGET_NONE)
  ######################################################################################
  # Logic configuration
  ######################################################################################
  MESSAGE("Adding C++11 flags, NEEDED to be supported by compiler")
  IF(TARGET_PLATFORM STREQUAL TARGET_APPLE)
      SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ ${CMAKE_CXX_FLAGS} ")
  ELSE()
    SET(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} ")
  ENDIF()

  ######################################################################################
  # Print info
  ######################################################################################
  PRINTADVANCEDINFO(${PROJ_MAIN_NAME})

  # 3rdParty
  MESSAGE("  3rdParty info, TODO" )
  MESSAGE("  ----------------------------------------")

  IF(${PROJ_NAME}_BUILD_TEST)
    MESSAGE(" Using ... for test, TODO")
  ENDIF()

  ######################################################################################
  # ADD SUB PROJECTS
  ######################################################################################
  #ADD_SUBDIRECTORY(3rdParty)
  ADD_SUBDIRECTORY(modules)

  IF(${PROJ_MAIN_NAME}_BUILD_SAMPLES)
    ADD_SUBDIRECTORY(samples)
  ENDIF()

  IF(${PROJ_MAIN_NAME}_BUILD_TEST)
    MESSAGE(" Included test generation.")
    ADD_SUBDIRECTORY(test)
  ENDIF()

ELSE()
  message(FATAL_ERROR " TARGET no autodetected. Please, select a TARGET manually. ")
ENDIF()
