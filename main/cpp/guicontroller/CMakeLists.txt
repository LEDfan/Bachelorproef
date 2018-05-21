if(Qt5_FOUND)
    set(GUICONTROLLER_SRC
            GuiController.cpp
            GuiControllerBackend.cpp
            SimWorker.cpp
    )

    qt5_add_resources(GUICONTROLLER_QT_RESOURCES controllerqml.qrc)

    #============================================================================
    # Build and link the guicontroller library.
    #============================================================================

    add_library(libguicontroller ${GUICONTROLLER_SRC} ${GUICONTROLLER_QT_RESOURCES})
    target_link_libraries(libguicontroller libgengeopop ${QT_LIBRARIES})

    unset(GUICONTROLLER_SRC)
    unset(GUICONTROLLER_QT_RESOURCES)
endif()
