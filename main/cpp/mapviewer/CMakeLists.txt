if(Qt5_FOUND)
    set(MAPVIEWER_SRC
        backend.cpp
        backends/LocationViewerBackend.cpp
        backends/ContactCenterViewerBackend.cpp
        models/ContactPoolListModel.cpp
        models/CommutesListModel.cpp
        models/ContactCenterListModel.cpp
        Map.cpp
        MapViewer.cpp
    )

    qt5_add_resources(MAPVIEWER_QT_RESOURCES qml.qrc)

    #============================================================================
    # Build and link the mapviewer library.
    #============================================================================
    add_library(libmapviewer ${MAPVIEWER_SRC} ${MAPVIEWER_QT_RESOURCES})
    target_link_libraries(libmapviewer libgengeopop ${QT_LIBRARIES})

    #============================================================================
    # Build and link the mapviewer executable.
    #============================================================================
    add_executable(mapviewer main.cpp)
    target_link_libraries(mapviewer libmapviewer)

    install(TARGETS mapviewer DESTINATION ${BIN_INSTALL_LOCATION})

    unset(MAPVIEWER_QT_RESOURCES)
    unset(MAPVIEWER_SRC)
endif()
