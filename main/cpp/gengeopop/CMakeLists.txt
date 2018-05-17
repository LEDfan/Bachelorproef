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
if(STRIDE_GEN_PROTO)
        include(FindProtobuf)
        find_package(Protobuf)
        if(${PROTOBUF_FOUND} AND NOT ${PROTOBUF_PROTOC_EXECUTABLE} STREQUAL "Protobuf_PROTOC_EXECUTABLE-NOTFOUND")
            file(GLOB proto_source "${CMAKE_CURRENT_SOURCE_DIR}/io/proto/*.proto")
            protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${proto_source})
            add_custom_target(proto
                COMMAND ${CMAKE_COMMAND} -E copy ${PROTO_HEADER} ${CMAKE_CURRENT_SOURCE_DIR}/io/proto/
                COMMAND ${CMAKE_COMMAND} -E copy ${PROTO_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/io/proto/
            )
        endif()
else()
        file(GLOB PROTO_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/io/proto/*.pb.h")
        file(GLOB PROTO_SRC "${CMAKE_CURRENT_SOURCE_DIR}/io/proto/*.pb.cc")
endif()
add_library(libproto ${PROTO_HEADER} ${PROTO_SRC})
target_link_libraries(libproto libprotobuf_insrc)

set_target_properties(libproto PROPERTIES COMPILE_FLAGS "-Wno-all -Wno-effc++ -Wno-extra -Wno-pedantic")
set_property(TARGET libproto PROPERTY POSITION_INDEPENDENT_CODE ON)

#============================================================================
# Build the GenGeoPop library.
#============================================================================
add_library(libgengeopop ${GENGEOPOP_LIB})
target_link_libraries(libgengeopop ${LIBS} libprotobuf_insrc libproto libstride)
set_property(TARGET libgengeopop PROPERTY POSITION_INDEPENDENT_CODE ON)

#============================================================================
# Build and install the stride executable.
#============================================================================
add_executable(gengeopop main.cpp)
target_link_libraries(gengeopop ${LIBS} libprotobuf_insrc libproto libstride libgengeopop)
install(TARGETS gengeopop DESTINATION ${BIN_INSTALL_LOCATION})

#============================================================================
# Clean up.
#============================================================================
unset(GENGEOPOP_LIB)

#############################################################################
