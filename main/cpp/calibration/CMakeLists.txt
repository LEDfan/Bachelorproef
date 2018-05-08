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
include_directories(calibration SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/tclap/include)

# Qt Charts
find_package(Qt5Charts)
if(Qt5Charts_FOUND)
    add_executable(calibration ${SRC} BoxplotGenerator.cpp)
    include_directories(${Qt5Charts_INCLUDE_DIRS})
    add_definitions(${Qt5Charts_DEFINITIONS})
    add_definitions(-DQTCHARTS)
    target_link_libraries(calibration ${LIBS} Qt5::Widgets Qt5::Charts libstride)
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
