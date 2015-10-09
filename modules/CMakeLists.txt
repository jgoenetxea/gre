MESSAGE("  |----------------------------------------------|")
MESSAGE("   Adding modules...")

  # Options
OPTION(${PROJ_MAIN_NAME}_BUILD_CORE_GRE			"Compile module CORE GRE" 								ON )
OPTION(${PROJ_MAIN_NAME}_BUILD_GREFX	 "Compile module GRE FX   "				    OFF )

  # Logic
IF(${PROJ_MAIN_NAME}_BUILD_CORE_GRE)
  ADD_SUBDIRECTORY(core)
ENDIF()

IF(${PROJ_MAIN_NAME}_BUILD_GREFX)
  ADD_SUBDIRECTORY(greFX)
ENDIF()

MESSAGE("   Modules added correctly!")
