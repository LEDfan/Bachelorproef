#============================================================================
# Build the Calibration executable
#============================================================================
set(SRC
    main.cpp
    Calibrator.cpp
    ../../../test/cpp/gtester/ScenarioData.cpp
)
add_executable(calibration ${SRC})
target_link_libraries(calibration ${LIBS} libstride)
install(TARGETS calibration DESTINATION ${BIN_INSTALL_LOCATION})

#============================================================================
# Clean up.
#============================================================================
unset(SRC)

#############################################################################
