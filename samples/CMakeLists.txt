# GUCpp
OPTION(${PROJ_MAIN_NAME}_DEPENDENCY_GUCPP     "Dependency sample" 				                      OFF )
IF(${PROJ_MAIN_NAME}_DEPENDENCY_GUCPP)
  MESSAGE("Adding GUCPP")
  SET(${PROJ_MAIN_NAME}_GUCPP_DIR NOT_FOUND CACHE PATH "The path where GUCPP is placed")
  IF(${PROJ_MAIN_NAME}_GUCPP_DIR STREQUAL NOT_FOUND)
        MESSAGE(FATAL_ERROR "Select GUCPP Dir manually!")
  ELSE()
        SET(${PROJ_MAIN_NAME}_GUCPP_INCLUDE_PATH
                        ${${PROJ_MAIN_NAME}_GUCPP_DIR}/geometry2d-tmb/include
                        ${${PROJ_MAIN_NAME}_GUCPP_DIR}/SimpleJSON/include )
        SET(${PROJ_MAIN_NAME}_GUCPP_LIBS_PATH
                        ${${PROJ_MAIN_NAME}_GUCPP_DIR}/geometry2d-tmb/lib
                        ${${PROJ_MAIN_NAME}_GUCPP_DIR}/SimpleJSON/lib)
        SET(${PROJ_MAIN_NAME}_GUCPP_LIBS geometry2d-tmb SimpleJSON)
  ENDIF()
  MESSAGE("---------------- GUCpp include path: ${${PROJ_MAIN_NAME}_GUCPP_INCLUDE_PATH}")
  MESSAGE("---------------- GUCpp libs path: ${${PROJ_MAIN_NAME}_GUCPP_LIBS_PATH}")
  MESSAGE("---------------- GUCpp libs: ${${PROJ_MAIN_NAME}_GUCPP_LIBS}")
ENDIF()

ADD_SUBDIRECTORY(shaderLab)
ADD_SUBDIRECTORY(BasicSceneManipulation)
ADD_SUBDIRECTORY(QuadAnimation)
