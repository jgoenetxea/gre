#---------------------------------------------------#
#          CMake Project Configuration              #
#---------------------------------------------------#
# CMake will issue a warning if you don't set this
cmake_minimum_required( VERSION 2.8 )

if(COMMAND cmake_policy)
     cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# If you don't name the project, it will take the default name "sample_aamqt"
# Note: You can modify the name of the project from CmakeGUI
SET(TEST_PROJECT_NAME test_example)
PROJECT( ${TEST_PROJECT_NAME} )

#---------------------------------------------------#
#              OpenCV Configuration                 #
#---------------------------------------------------#
# Automatically find OpenCV files


#---------------------------------------------------#
#               Project Configuration               #
#---------------------------------------------------#
SET ( CMAKE_DEBUG_POSTFIX d )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/core/include )
link_directories( ${CMAKE_BINARY_DIR}/bin )

#---------------------------------------------------#
#               Testing Configuration               #
#---------------------------------------------------#
SET(TESTING_FRAMEWORK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../catch_testing_framework)
#MESSAGE("The path to testing framework is: ${TESTING_HEADERS}")
INCLUDE_DIRECTORIES(${TESTING_FRAMEWORK_DIR}/include)

#---------------------------------------------------#
#    Files of the project .cpp & .h & moc files     #
#---------------------------------------------------#
SET( PROJECT_SRCS
	test_example.cpp
)

SET( PROJECT_HEADERS
)

#---------------------------------------------------#
# 			EXECUTABLE	            #
#---------------------------------------------------#
ADD_EXECUTABLE( ${TEST_PROJECT_NAME}  ${PROJECT_SRCS}  ${PROJECT_HEADERS} )

#---------------------------------------------------#
#               Project Configuration               #
#---------------------------------------------------#
SET_TARGET_PROPERTIES( ${TEST_PROJECT_NAME} PROPERTIES FOLDER "gre_tests")
TARGET_LINK_LIBRARIES( ${TEST_PROJECT_NAME}  debug gre_core_d optimized gre_core )

#---------------------------------------------------#
#                Add automatic test                 #
#---------------------------------------------------#

IF( GRE_TEST_RUN_AUTOMATIC )
  ADD_TEST( RUN_test_example  ${EXECUTABLE_OUTPUT_PATH}/${TEST_PROJECT_NAME} )
ENDIF( GRE_TEST_RUN_AUTOMATIC )
