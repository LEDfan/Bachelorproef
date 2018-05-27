if(Qt5_FOUND)
    set(GUILAUNCHER_SRC
        main.cpp
        Launcher.cpp
        Launcher.h
    )

    qt5_add_resources(GUILAUNCHER_QT_RESOURCES launcherqml.qrc)

    #============================================================================
    # Build and link the guilauncher executable.
    #============================================================================
    add_executable(guilauncher ${GUILAUNCHER_SRC} ${GUILAUNCHER_QT_RESOURCES})
    target_link_libraries(guilauncher libgengeopop libmapviewer ${QT_LIBRARIES})

    install(TARGETS guilauncher DESTINATION ${BIN_INSTALL_LOCATION})

    unset(GUILAUNCHER_QT_RESOURCES)
    unset(GUILAUNCHER_SRC)
endif()

