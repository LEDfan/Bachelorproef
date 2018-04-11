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
#  Copyright 2017, Willem L, Kuylen E, Stijven S & Broeckhove J
#############################################################################


#============================================================================
# Build & install the (OpenMP enabled if OpenMP available) executable.
#============================================================================
set(EXEC gtester)
set(SRC
        BatchRuns.cpp
        CSVTest.cpp
        ScenarioData.cpp
        UnitSegmentedVector.cpp
        gengeopop/generators/CommunityGeneratorTest.cpp
        gengeopop/generators/HighSchoolGeneratorTest.cpp
        gengeopop/generators/HouseholdGeneratorTest.cpp
        gengeopop/generators/SchoolGeneratorTest.cpp
        gengeopop/generators/WorkplaceGeneratorTest.cpp
        gengeopop/io/CitiesCSVReaderTest.cpp
        gengeopop/io/CommutesCSVReaderTest.cpp
        gengeopop/io/GeoGridJSONWriterTest.cpp
        gengeopop/io/GeoGridProtoWriterTest.cpp
        gengeopop/io/GeoGridProtoReaderTest.cpp
        gengeopop/io/HouseholdCSVReaderTest.cpp
        gengeopop/io/ReaderFactoryTest.cpp
        gengeopop/io/SubMunicipalitiesCSVReaderTest.cpp
        gengeopop/io/GeoGridIOUtils.cpp
        gengeopop/kdtree.cpp
        main.cpp)

if (OPENMP_FOUND)
        set(SRC ${SRC} ParallelRuns.cpp)
endif ()

include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/tclap/include ${CMAKE_HOME_DIRECTORY}/main/resources/lib/protobuf)

add_executable(${EXEC} ${SRC})
target_link_libraries(${EXEC} ${LIBS} gtest pthread libstride libgengeopop)
install(TARGETS ${EXEC} DESTINATION ${BIN_INSTALL_LOCATION})
install(DIRECTORY testdata/ DESTINATION ${TESTS_INSTALL_LOCATION}/testdata/)

#============================================================================
# Define tests.
#============================================================================
add_test(NAME ${EXEC}_all
        WORKING_DIRECTORY ${TESTS_DIR}
        COMMAND ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_LOCATION}/${EXEC}
        --gtest_output=xml:gtester_all.xml
        )
#============================================================================
# Clean up.
#============================================================================
unset(EXEC)
unset(SRC)

#############################################################################
