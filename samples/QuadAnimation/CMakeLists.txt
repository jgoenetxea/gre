# Header
#-----------------------------------------------------------------------#
SET(PROJ_NAME QuadAnimation)
PROJECT(${PROJ_NAME})
MESSAGE(" + Adding module ${PROJ_NAME} ")				# In info


FIND_PACKAGE( GLFW REQUIRED )


# Include directories with headers
#-----------------------------------------------------------------------#
INCLUDE_DIRECTORIES( ${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/core/include )	# Other includes in modules
INCLUDE_DIRECTORIES( ${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/unnamed/include )	# Other includes in modules
INCLUDE_DIRECTORIES( ${GLM_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${GLFW_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${${PROJ_MAIN_NAME}_GUCPP_INCLUDE_PATH} )

LINK_DIRECTORIES( ${${PROJ_MAIN_NAME}_GUCPP_LIBS_PATH} )
LINK_DIRECTORIES( ${GLFW_LIBRARY_DIRS})

# Locate the main files of each sample.
#-----------------------------------------------------------------------#
SET( SRC main.cpp
         rectGenerator.h
         rectGenerator.cpp
         glfwWindow.h
         glfwWindow.cpp )


# Create Library
#-----------------------------------------------------------------------#
ADD_DEFINITIONS(-DASSET_DIRECTORY="${CMAKE_SOURCE_DIR}/samples/QuadAnimation/assets/" )

ADD_EXECUTABLE( ${PROJ_NAME} ${SRC} )
TARGET_LINK_LIBRARIES( ${PROJ_NAME}
                                    gre_core
                                    gre_unnamed
                                    ${GLFW_LIBRARIES}
                                    ${OPENGL_gl_LIBRARY}
                                    ${${PROJ_MAIN_NAME}_GUCPP_LIBS}
                                    )

# Install
#-----------------------------------------------------------------------#
#INSTALL(TARGETS ${PROJ_NAME}
#        DESTINATION "${${PROJ_MAIN_NAME}_PATH_INSTALL}/${PROJ_NAME}/lib"
#        )
#INSTALL(FILES ${${PROJ_NAME}_HEADERS}
#        DESTINATION "${${PROJ_MAIN_NAME}_PATH_INSTALL}/${PROJ_NAME}/include"
#        )
#
PRINTBASICINFO(${PROJ_NAME})						# Macro cmake_tools to print, uncomment to add info
#MESSAGE(" - Module ${PROJ_NAME} added!")					# Out info
