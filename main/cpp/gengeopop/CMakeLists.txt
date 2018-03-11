#============================================================================
# Sources.
#============================================================================
set(GENGEOPOP_LIB
    Community.cpp
    ContactCenter.cpp
    ContactPool.cpp
    GeoGrid.cpp
    HighSchool.cpp
    Household.cpp
    Location.cpp
    School.cpp
    Workplace.cpp
    generators/GeoGridGenerator.cpp
    generators/SchoolGenerator.cpp
    generators/IPartialGenerator.h
    readers/CitiesCSVReader.cpp
    readers/CitiesReader.cpp
    readers/CommutesReader.cpp
    readers/HouseholdCSVReader.cpp
    readers/HouseholdReader.cpp
    readers/ReaderFactory.cpp

    # TOOD remove header files after initial implementation
    Community.h
    HighSchool.h
    School.h
    Workplace.h
    generators/GeoGridGenerator.h
    generators/IPartialGenerator.h
    generators/SchoolGenerator.h
    readers/CitiesCSVReader.h
    readers/HouseholdCSVReader.h
    readers/HouseholdReader.h
    GeoGridConfig.h
)

#============================================================================
# Build the GenGeoPop library.
#============================================================================
add_library(libgengeopop ${GENGEOPOP_LIB})
target_link_libraries(libgengeopop ${LIBS} libstride)
target_compile_options(libgengeopop PUBLIC "-fPIC")
set_target_properties(libgengeopop PROPERTIES PREFIX "")

#============================================================================
# Build and install the stride executable.
#============================================================================
add_executable(gengeopop main.cpp)
target_include_directories(gengeopop PUBLIC ${CMAKE_HOME_DIRECTORY}/main/resources/lib/tclap/include)
target_link_libraries(gengeopop ${LIBS} libstride libgengeopop)
install(TARGETS gengeopop DESTINATION  ${BIN_INSTALL_LOCATION})


#============================================================================
# Clean up.
#============================================================================
unset(GENGEOPOP_LIB)

#############################################################################
