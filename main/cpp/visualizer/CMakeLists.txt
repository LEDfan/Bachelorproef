
set(VISUALIZER_SRC
    backend.cpp
#    backend.h
    main.cpp
    HomeForm.ui.qml
)

find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})
qt5_add_resources(QT_RESOURCES qml.qrc)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11 -fstrict-aliasing -pedantic-errors -pedantic -Wno-deprecated-declarations -Wno-unused-variable")

#source_group("Header Files" FILES ${HEADERS})
#source_group("Source Files" FILES ${SOURCES})
add_executable(visualizer ${VISUALIZER_SRC} ${QT_RESOURCES})
target_link_libraries(visualizer
    libgengeopop
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
)

install(TARGETS visualizer DESTINATION   ${BIN_INSTALL_LOCATION})

unset(VISUALIZER_SRC)
