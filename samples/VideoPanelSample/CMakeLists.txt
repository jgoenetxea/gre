# Header
#-----------------------------------------------------------------------#
SET(PROJ_NAME VideoPanelSample)
PROJECT(${PROJ_NAME})
MESSAGE(" + Adding module ${PROJ_NAME} ")				# In info

FIND_PACKAGE( GLFW REQUIRED )
FIND_PACKAGE( X11 REQUIRED )
FIND_PACKAGE( OpenCV REQUIRED )

# Include directories with headers
#-----------------------------------------------------------------------#
INCLUDE_DIRECTORIES( ${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/core/include )	# Other includes in modules
INCLUDE_DIRECTORIES( ${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/video_panel/include )	# Other includes in modules
INCLUDE_DIRECTORIES( ${GLM_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${GLFW_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${${PROJ_MAIN_NAME}_GUCPP_INCLUDE_PATH} )
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )

LINK_DIRECTORIES( ${GLFW_LIBRARY_DIRS})
LINK_DIRECTORIES( ${${PROJ_MAIN_NAME}_GUCPP_LIBS_PATH} )

# Locate the main files of each sample.
#-----------------------------------------------------------------------#
SET( SRC main.cpp )

# Create Library
#-----------------------------------------------------------------------#
ADD_DEFINITIONS(-DASSET_DIRECTORY="${CMAKE_SOURCE_DIR}/samples/BasicSceneManipulation/assets/" )

ADD_EXECUTABLE( ${PROJ_NAME} ${SRC} )
TARGET_LINK_LIBRARIES( ${PROJ_NAME} gre_core gre_video_panel ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} ${OpenCV_LIBS} )

#PRINTBASICINFO(${PROJ_NAME})						# Macro cmake_tools to print, uncomment to add info
MESSAGE(" - Samples ${PROJ_NAME} done!")					# Out info
