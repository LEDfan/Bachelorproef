set(VISUALIZER_SRC
    backend.cpp
    backends/LocationViewerBackend.cpp
    backends/ContactCenterViewerBackend.cpp
    models/ContactPoolListModel.cpp
    models/CommutesListModel.cpp
    models/ContactCenterListModel.cpp
    Visualizer.cpp Visualizer.h
)

set (CMAKE_CXX_STANDARD 14)

qt5_add_resources(VISUALIZER_QT_RESOURCES qml.qrc)

#============================================================================
# Build and link the visualizer library.
#============================================================================
add_library(libvis ${VISUALIZER_SRC} ${VISUALIZER_QT_RESOURCES})
target_link_libraries(libvis
    libgengeopop
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
)

#============================================================================
# Build and link the visualizer executable.
#============================================================================
add_executable(visualizer main.cpp)
target_link_libraries(visualizer libvis)

install(TARGETS visualizer DESTINATION ${BIN_INSTALL_LOCATION})

unset(VISUALIZER_SRC)
unset(VISUALIZER_QT_RESOURCES)
