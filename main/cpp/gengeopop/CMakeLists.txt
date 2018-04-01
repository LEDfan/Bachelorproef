#============================================================================
# Sources.
#============================================================================
set(GENGEOPOP_LIB
        Coordinate.cpp
        Community.cpp
        PrimaryCommunity.cpp
        SecondaryCommunity.cpp
        ContactCenter.cpp
        ContactPool.cpp
        GeoGrid.cpp
        HighSchool.cpp
        Household.cpp
        Location.cpp
        School.cpp
        Workplace.cpp
        GeoGridConfig.cpp
        generators/GeoGridGenerator.cpp
        generators/SchoolGenerator.cpp
        generators/PartialGenerator.cpp
        generators/HighSchoolGenerator.cpp
        generators/HouseholdGenerator.cpp
        generators/WorkplaceGenerator.cpp
        generators/CommunityGenerator.cpp
        populators/PrimaryCommunityPopulator.cpp
        populators/SecondaryCommunityPopulator.cpp
        populators/GeoGridPopulator.cpp
        populators/HouseholdPopulator.cpp
        populators/PartialPopulator.cpp
        populators/SchoolPopulator.cpp
        populators/HighSchoolPopulator.cpp
        io/CitiesCSVReader.cpp
        io/CitiesReader.cpp
        io/CommutesReader.cpp
        io/CommutesCSVReader.cpp
        io/HouseholdCSVReader.cpp
        io/HouseholdReader.cpp
        io/ReaderFactory.cpp
        io/GeoGridJSONWriter.cpp
        io/GeoGridJSONReader.cpp)

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
include_directories(gengeopop SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/tclap/include)
target_link_libraries(gengeopop ${LIBS} libstride libgengeopop)
install(TARGETS gengeopop DESTINATION ${BIN_INSTALL_LOCATION})


#============================================================================
# Clean up.
#============================================================================
unset(GENGEOPOP_LIB)

#############################################################################
