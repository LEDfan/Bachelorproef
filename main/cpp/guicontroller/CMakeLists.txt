set(GUICONTROLLER_SRC
        main.cpp
        GuiController.cpp
        GuiControllerBackend.cpp
)
set (CMAKE_CXX_STANDARD 14)


#============================================================================
# Find dependencies and correct flags.
#============================================================================
find_package(Qt5 COMPONENTS Widgets Qml Quick QuickTest REQUIRED)
include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})
qt5_add_resources(QT_RESOURCES controllerqml.qrc)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#============================================================================
# Build and link the guicontroller executable.
#============================================================================

add_library(libguicontroller ${GUICONTROLLER_SRC} ${QT_RESOURCES})
target_link_libraries(libguicontroller
    libgengeopop
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
)

unset(GUICONTROLLER_SRC)
