# Header
#-----------------------------------------------------------------------#
SET(PROJ_NAME sample-shaderLab)
PROJECT(${PROJ_NAME})
MESSAGE(" + Adding module ${PROJ_NAME} ")				# In info

# Include directories with headers
#-----------------------------------------------------------------------#
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include )		# This projects' includes
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/scenes )		# This projects' scenes
INCLUDE_DIRECTORIES( ${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/gre/include )	# Other includes in modules
INCLUDE_DIRECTORIES( ${GLM_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )

# Source, macro to find all files. If you add more, regenerate project
#-----------------------------------------------------------------------#
FILE(GLOB ${PROJ_NAME}_SRC
    "src/*.cpp"
)
FILE(GLOB ${PROJ_NAME}_HEADERS
    "include/*.h"
)
FILE(GLOB ${PROJ_NAME}_SCENES
    "scenes/*.h"
    "scenes/*.cpp"
)

# Qt resources
SET( PROJECT_FORMS
	forms/mainwindow.ui
	)
SET( PROJECT_RESOURCES resources/images.qrc )

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

#---------------------------------------------------#
#              Qt5 Configuration                    #
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






# Create Library
#-----------------------------------------------------------------------#
ADD_EXECUTABLE( ${PROJ_NAME} 
			${${PROJ_NAME}_HEADERS} 
			${${PROJ_NAME}_SRC} 
			${PROJECT_FORMS_HEADERS} 
			${PROJECT_RESOURCES_RCC}
) 

# Link libraries with Project
#-----------------------------------------------------------------------#
TARGET_LINK_LIBRARIES( ${PROJ_NAME} gre-l)
TARGET_LINK_LIBRARIES( ${PROJ_NAME} ${OPENGL_LIBRARIES} )
TARGET_LINK_LIBRARIES( ${PROJ_NAME} ${Qt5Widgets_LIBRARIES} )
TARGET_LINK_LIBRARIES( ${PROJ_NAME} ${Qt5OpenGL_LIBRARIES} )


# Install
#-----------------------------------------------------------------------#
INSTALL(TARGETS ${PROJ_NAME}
        DESTINATION "${${PROJ_MAIN_NAME}_PATH_INSTALL}/${PROJ_NAME}/lib"
        )
INSTALL(FILES ${${PROJ_NAME}_HEADERS}
        DESTINATION "${${PROJ_MAIN_NAME}_PATH_INSTALL}/${PROJ_NAME}/include"
        )

#PRINTBASICINFO(${PROJ_NAME})						# Macro cmake_tools to print, uncomment to add info
MESSAGE(" - Module ${PROJ_NAME} added!")					# Out info
