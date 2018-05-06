#############################################################################
#  This file is part of the Stride software.
#  It is free software: you can redistribute it and/or modify
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
#  Copyright 2017, 2018 Willem L, Kuylen E & Broeckhove J
#############################################################################

#============================================================================
# Proto.
#============================================================================
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/gengeopop/io/proto)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../resources/lib/protobuf)

#============================================================================
# Subdirectories.
#============================================================================
add_subdirectory(gengeopop)
add_subdirectory(calibration)

#============================================================================
# Sources. Includes headers to enable code insight by CLion.
#============================================================================
set(LIB_SRC
        #---
        behaviour/behaviour_policies/NoBehaviour.h
        behaviour/behaviour_policies/Vaccination.h
        behaviour/belief_policies/Belief.h
        behaviour/belief_policies/HBM.h
        behaviour/belief_policies/Imitation.h
        behaviour/belief_policies/NoBelief.h
        behaviour/belief_policies/Threshold.h
        behaviour/information_policies/LocalDiscussion.h
        behaviour/information_policies/NoGlobalInformation.h
        behaviour/information_policies/NoLocalInformation.h
        #---
        calendar/Calendar.cpp
        calendar/DaysOffAll.h
        calendar/DaysOffInterface.h
        calendar/DaysOffNone.h
        calendar/DaysOffSchool.h
        calendar/DaysOffStandard.h
        #---
        contact/AgeContactProfile.cpp
        contact/AgeContactProfiles.h
        contact/ContactHandler.h
        contact/ContactLogMode.cpp
        contact/Infector.cpp
        contact/InfectorExec.h
        contact/InfectorMap.h
        contact/LogPolicy.h
        contact/TransmissionProfile.cpp
        #---
        disease/DiseaseSeeder.cpp
        disease/Health.cpp
        disease/HealthSeeder.cpp
        disease/Immunizer.cpp
        #---
        output/AdoptedFile.cpp
        output/PersonsFile.cpp
        output/SummaryFile.cpp
        output/InfectedFile.cpp
        #---
        pool/ContactPool.cpp
        pool/ContactPoolSys.h
        pool/ContactPoolType.cpp
        pool/IdSubscriptArray.h
        #---
        pop/Age.h
        pop/Person.cpp
        pop/Population.cpp
        pop/DefaultPopBuilder.cpp
        pop/AbstractPopBuilder.cpp
        pop/GenPopBuilder.cpp
        pop/SurveySeeder.cpp
        #---
        sim/CliController.cpp
        sim/CliWithVisualizerController.cpp
        sim/SimRunner.cpp
        sim/SimBuilder.cpp
        sim/Sim.cpp
        sim/event/Id.cpp
        sim/event/Subject.cpp
        #---
        util/Any.h
        util/ConfigInfo.cpp
        util/CSV.cpp
        util/CSVRow.cpp
        util/FileSys.cpp
        util/LogUtils.cpp
        util/RNEngineType.cpp
        util/RNManager.cpp
        util/RunConfigManager.cpp
        util/SegmentedVector.h
        util/Stopwatch.h
        util/StringUtils.h
        util/Subject.h
        util/SVIterator.h
        util/TimeStamp.h
        util/TimeToString.h
        util/ExcAssert.h
        util/Exception.cpp
        #---
        viewers/AdoptedViewer.cpp
        viewers/CliViewer.cpp
        viewers/PersonsViewer.cpp
        viewers/SummaryViewer.cpp
        viewers/MapViewer.cpp
        viewers/MapViewer.h
        viewers/InfectedViewer.cpp
		#---
        )

#============================================================================
# Build the Stride library.
#============================================================================
add_library(libstride ${LIB_SRC})
target_link_libraries(libstride ${LIBS} libgengeopop)
target_compile_options(libstride PUBLIC "-fPIC")
target_compile_definitions(libstride PUBLIC "-DSTRIDE_GIT_HASH=${STRIDE_WC_REVISION_HASH}")
set_target_properties(libstride PROPERTIES PREFIX "")

#============================================================================
# Build and install the stride executable.
#============================================================================
# QT related
find_package(Qt5 COMPONENTS Widgets Qml Quick QuickTest REQUIRED)
include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_executable(stride sim/main.cpp)
include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/tclap/include)
target_link_libraries(stride ${LIBS} libstride libgengeopop)
target_link_libraries(stride libvis)
install(TARGETS stride DESTINATION ${BIN_INSTALL_LOCATION})

add_subdirectory(visualizer)

#============================================================================
# Clean up.
#============================================================================
unset(LIB_SRC)

#############################################################################
