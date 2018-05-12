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
        College.cpp
        Household.cpp
        Location.cpp
        K12School.cpp
        Workplace.cpp
        GeoGridConfig.cpp
        generators/GeoGridGenerator.cpp
        generators/K12SchoolGenerator.cpp
        generators/PartialGenerator.cpp
        generators/CollegeGenerator.cpp
        generators/HouseholdGenerator.cpp
        generators/WorkplaceGenerator.cpp
        generators/CommunityGenerator.cpp
        populators/PrimaryCommunityPopulator.cpp
        populators/SecondaryCommunityPopulator.cpp
        populators/GeoGridPopulator.cpp
        populators/HouseholdPopulator.cpp
        populators/PartialPopulator.cpp
        populators/K12SchoolPopulator.cpp
        populators/CollegePopulator.cpp
        populators/WorkplacePopulator.cpp
        io/CitiesCSVReader.cpp
        io/CitiesReader.cpp
        io/CommutesReader.cpp
        io/CommutesCSVReader.cpp
        io/HouseholdCSVReader.cpp
        io/HouseholdReader.cpp
        io/ReaderFactory.cpp
        io/GeoGridJSONWriter.cpp
        io/GeoGridJSONReader.cpp
        io/GeoGridProtoWriter.cpp
        io/GeoGridProtoReader.cpp
        io/GeoGridWriterFactory.cpp
        io/GeoGridReaderFactory.cpp
        io/GeoGridReader.cpp
        io/SubMunicipalitiesReader.cpp
        io/SubMunicipalitiesCSVReader.cpp
        GenGeoPopController.cpp)

#============================================================================
# Protocol Buffers.
#============================================================================
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf)
if(${PROTOBUF_FOUND} AND NOT ${PROTOBUF_PROTOC_EXECUTABLE} STREQUAL "Protobuf_PROTOC_EXECUTABLE-NOTFOUND")
    file(GLOB proto_source "${CMAKE_CURRENT_SOURCE_DIR}/io/proto/*.proto")
    PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ${proto_source})
    add_library(libproto ${PROTO_HEADER} ${PROTO_SRC})
    add_custom_target(proto DEPENDS libproto)
    add_custom_command(
        TARGET proto
        COMMAND ${CMAKE_COMMAND} -E copy ${PROTO_HEADER} ${CMAKE_CURRENT_SOURCE_DIR}/io/proto/
        COMMAND ${CMAKE_COMMAND} -E copy ${PROTO_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/io/proto/
    )
else()
    file(GLOB PROTO_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/io/proto/*.pb.h")
    file(GLOB PROTO_SRC "${CMAKE_CURRENT_SOURCE_DIR}/io/proto/*.pb.cc")
    add_library(libproto ${PROTO_HEADER} ${PROTO_SRC})
endif()
set_target_properties(libproto PROPERTIES COMPILE_FLAGS "-Wno-all -Wno-effc++ -Wno-extra -Wno-pedantic")
set_property(TARGET libproto PROPERTY POSITION_INDEPENDENT_CODE ON)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/io/proto)

#============================================================================
# Build the GenGeoPop library.
#============================================================================
add_library(libgengeopop ${GENGEOPOP_LIB})
target_link_libraries(libgengeopop ${LIBS} libprotobuf libproto libstride)
set_property(TARGET libgengeopop PROPERTY POSITION_INDEPENDENT_CODE ON)

#============================================================================
# Build and install the stride executable.
#============================================================================
add_executable(gengeopop main.cpp)
target_link_libraries(gengeopop ${LIBS} libprotobuf libproto libstride libgengeopop)
install(TARGETS gengeopop DESTINATION ${BIN_INSTALL_LOCATION})


#============================================================================
# Clean up.
#============================================================================
unset(GENGEOPOP_LIB)

#############################################################################
