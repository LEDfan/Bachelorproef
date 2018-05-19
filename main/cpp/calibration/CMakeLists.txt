#============================================================================
# Build the Calibration executable
#============================================================================
set(SRC
    main.cpp
    Calibrator.cpp
    Boxplot.cpp
    TestCalibrationRunner.cpp
    ConfigFileCalibrationRunner.cpp
    CalibrationRunner.cpp
    ../../../test/cpp/gtester/ScenarioData.cpp
)

if(Qt5Charts_FOUND)
    add_executable(calibration ${SRC} BoxplotGenerator.cpp)
    target_link_libraries(calibration ${LIBS} ${QT_LIBRARIES} libstride)
else()
    add_executable(calibration ${SRC})
    target_link_libraries(calibration ${LIBS} libstride)
endif()

install(TARGETS calibration DESTINATION ${BIN_INSTALL_LOCATION})

#============================================================================
# Clean up.
#============================================================================
unset(SRC)

#############################################################################
