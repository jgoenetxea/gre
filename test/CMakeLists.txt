#-----------------------------------------------------------------------------
# Turn on testing support. This will generate the RUN_TESTS project in VS
ENABLE_TESTING()
SET ( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/ CACHE PATH "Single output directory for building all executables." FORCE )
OPTION(GRE_TEST_RUN_AUTOMATIC OFF)

ADD_SUBDIRECTORY( core )
