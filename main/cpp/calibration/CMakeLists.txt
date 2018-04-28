#============================================================================
# Build the Calibration executable
#============================================================================
set(SRC
    main.cpp
    Calibrator.cpp
    Boxplot.cpp
    ../../../test/cpp/gtester/ScenarioData.cpp
)

# Qt Charts
find_package(Qt5Charts)
include_directories(${Qt5Charts_INCLUDE_DIRS})
add_definitions(${Qt5Charts_DEFINITIONS})

add_executable(calibration ${SRC})
include_directories(calibration SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/tclap/include)
target_link_libraries(calibration ${LIBS} Qt5::Widgets Qt5::Charts libstride)
install(TARGETS calibration DESTINATION ${BIN_INSTALL_LOCATION})

#============================================================================
# Clean up.
#============================================================================
unset(SRC)

#############################################################################
