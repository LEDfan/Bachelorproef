#############################################################################
#  This is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or any
#  later version.
#  The software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License,
#  along with the software. If not, see <http://www.gnu.org/licenses/>.
#  see http://www.gnu.org/licenses/.
#
#  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
#############################################################################

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
