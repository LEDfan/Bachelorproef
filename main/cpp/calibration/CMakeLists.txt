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
#  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
#############################################################################

#============================================================================
# Build the Calibration executable
#============================================================================
set(SRC
    Calibrator.cpp
    Boxplot.cpp
    TestCalibrationRunner.cpp
    ConfigFileCalibrationRunner.cpp
    CalibrationRunner.cpp
    ../../../test/cpp/gtester/ScenarioData.cpp
)

if(Qt5Charts_FOUND)
    add_library(libcalibration ${SRC} BoxplotGenerator.cpp)
    target_compile_options(libcalibration PUBLIC "-fPIC")
else()
    add_library(libcalibration ${SRC})
endif()


#============================================================================
# Clean up.
#============================================================================
unset(SRC)

#############################################################################
