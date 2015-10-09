# Header
#-----------------------------------------------------------------------#
SET(PROJ_NAME sample-list)
PROJECT(${PROJ_NAME})
MESSAGE(" + Adding module ${PROJ_NAME} ")				# In info


FIND_PACKAGE( GLFW REQUIRED )
FIND_PACKAGE( X11 REQUIRED )

# Include directories with headers
#-----------------------------------------------------------------------#
INCLUDE_DIRECTORIES( ${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/gre/include )	# Other includes in modules
INCLUDE_DIRECTORIES( ${GLM_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${GLFW_INCLUDE_DIRS} )

LINK_DIRECTORIES( ${GLFW_LIBRARY_DIRS})


# Locate the main files of each sample.
#-----------------------------------------------------------------------#
SET( TESTING_BASE_APP basic/main.cpp )
SET( TESTING_BASE_64k_APP basic_64k/main.cpp )
SET( TESTING_BASE_ORTHO_APP basicOrtho/main.cpp )
SET( TESTING_ORTHO_PANEL_APP orthoPanel/main.cpp )
SET( MULTY_SCENE_APP sceneSwitcher/main.cpp )
SET( EXPLOSION_SCENE_APP explosionScene/main.cpp )

SET( GLFW_SAMPLE glfw_sample/main.cpp )


# Create Library
#-----------------------------------------------------------------------#
ADD_EXECUTABLE( sample_basicCubeMoving ${TESTING_BASE_APP} )
TARGET_LINK_LIBRARIES( sample_basicCubeMoving gre-l ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} )

ADD_EXECUTABLE( sample_glfw ${GLFW_SAMPLE} )
TARGET_LINK_LIBRARIES( sample_glfw ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} )

#ADD_EXECUTABLE( sample_basicCubeMoving_64k  ${TESTING_BASE_64k_APP} )
#TARGET_LINK_LIBRARIES( sample_basicCubeMoving_64k gre-l ${OPENGL_gl_LIBRARY} )

#ADD_EXECUTABLE( sample_basicOrtho ${TESTING_BASE_ORTHO_APP} )
#TARGET_LINK_LIBRARIES( sample_basicOrtho gre-l ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} )

#ADD_EXECUTABLE( sample_basicOrthoPanel ${TESTING_ORTHO_PANEL_APP} )
#TARGET_LINK_LIBRARIES( sample_basicOrthoPanel gre-l ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} )

#ADD_EXECUTABLE( sample_multyScene ${MULTY_SCENE_APP} )
#TARGET_LINK_LIBRARIES( sample_multyScene gre-l ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} )


# Install
#-----------------------------------------------------------------------#
#INSTALL(TARGETS ${PROJ_NAME}
#        DESTINATION "${${PROJ_MAIN_NAME}_PATH_INSTALL}/${PROJ_NAME}/lib"
#        )
#INSTALL(FILES ${${PROJ_NAME}_HEADERS}
#        DESTINATION "${${PROJ_MAIN_NAME}_PATH_INSTALL}/${PROJ_NAME}/include"
#        )
#
#PRINTBASICINFO(${PROJ_NAME})						# Macro cmake_tools to print, uncomment to add info
#MESSAGE(" - Module ${PROJ_NAME} added!")					# Out info
