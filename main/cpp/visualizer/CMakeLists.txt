set(VISUALIZER_SRC
    backend.cpp
    main.cpp
    backends/LocationViewerBackend.cpp
    backends/ContactCenterViewerBackend.cpp
    models/ContactPoolListModel.cpp
    models/ContactCenterListModel.cpp
        )

set(TEST_SRC
        tests/tests.cpp
        tests/tst_basic.qml)
set (CMAKE_CXX_STANDARD 14)

find_package(Qt5 COMPONENTS Widgets Qml Quick QuickTest REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})
qt5_add_resources(QT_RESOURCES qml.qrc)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
add_executable(visualizer ${VISUALIZER_SRC} ${QT_RESOURCES})
target_link_libraries(visualizer
    libgengeopop
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
)

set(QUICK_TEST_SOURCE_DIR tests)
enable_testing()
set(CMAKE_INCLUDE_CURRENT_DIR ON)
qt5_add_resources(QT_RESOURCES qml.qrc)
add_executable(vis_test ${TEST_SRC} ${QT_RESOURCES})
add_test(vis_test vis_test)

find_package(Qt5Test REQUIRED)
target_link_libraries(vis_test
    libgengeopop
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickTest
    Qt5::Test
)

install(TARGETS visualizer DESTINATION   ${BIN_INSTALL_LOCATION})
install(TARGETS vis_test DESTINATION   ${BIN_INSTALL_LOCATION})

enable_testing()
add_test( NAME visualizer_test
    COMMAND ${BIN_INSTALL_LOCATION}/vis_test
)

unset(VISUALIZER_SRC)
unset(TEST_SRC)
