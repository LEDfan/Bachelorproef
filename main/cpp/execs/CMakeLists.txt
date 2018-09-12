#============================================================================
# Build the Stride binary.
#============================================================================
add_executable(stride stride.cpp)

if(Qt5_FOUND)
    target_link_libraries(stride ${LIBS} libstride libgengeopop libmapviewer libguicontroller)
else()
    target_link_libraries(stride ${LIBS} libstride libgengeopop)
endif()
install(TARGETS stride DESTINATION ${BIN_INSTALL_LOCATION})

#============================================================================
# Build the Calibration binary.
#============================================================================
add_executable(calibration calibration.cpp)

if(Qt5_FOUND)
    target_link_libraries(calibration ${LIBS} ${QT_LIBRARIES} libstride libcalibration)
else()
    target_link_libraries(calibration ${LIBS} libstride libcalibration)
endif()

install(TARGETS calibration DESTINATION ${BIN_INSTALL_LOCATION})

#============================================================================
# Build the GuiLauncher binary.
#============================================================================

if(Qt5_FOUND)
    add_executable(guilauncher guilauncher.cpp)
    target_link_libraries(guilauncher libguilauncher libgengeopop libmapviewer ${QT_LIBRARIES})
    install(TARGETS guilauncher DESTINATION ${BIN_INSTALL_LOCATION})
endif()

#============================================================================
# Build the GenGeoPop binary.
#============================================================================
add_executable(gengeopop gengeopop.cpp)
target_link_libraries(gengeopop ${LIBS} libprotobuf_insrc libproto libstride libgengeopop)
install(TARGETS gengeopop DESTINATION ${BIN_INSTALL_LOCATION})

#============================================================================
# Build the MapViewer binary.
#============================================================================
if(Qt5_FOUND)
    add_executable(mapviewer mapviewer.cpp)
    target_link_libraries(mapviewer libmapviewer)
    install(TARGETS mapviewer DESTINATION ${BIN_INSTALL_LOCATION})
endif()
