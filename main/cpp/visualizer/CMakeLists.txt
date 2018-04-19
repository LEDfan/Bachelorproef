set(VISUALIZER_SRC
    backend.cpp
    main.cpp
    backends/LocationViewerBackend.cpp
    backends/ContactCenterViewerBackend.cpp
    models/ContactPoolListModel.cpp
    models/CommutesListModel.cpp
    models/ContactCenterListModel.cpp
)

set (CMAKE_CXX_STANDARD 14)

#============================================================================
# Find dependencies and correct flags.
#============================================================================
find_package(Qt5 COMPONENTS Widgets Qml Quick QuickTest REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})
qt5_add_resources(QT_RESOURCES qml.qrc)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#============================================================================
# Build and link the visualizer executable.
#============================================================================
add_executable(visualizer ${VISUALIZER_SRC} ${QT_RESOURCES})
target_link_libraries(visualizer
    libgengeopop
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
)

install(TARGETS visualizer DESTINATION   ${BIN_INSTALL_LOCATION})

unset(VISUALIZER_SRC)
