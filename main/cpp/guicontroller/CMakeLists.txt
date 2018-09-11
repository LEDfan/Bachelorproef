#############################################################################
#  This is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or any
#  later version.
#  The software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License,
#  along with the software. If not, see <http://www.gnu.org/licenses/>.
#  see http://www.gnu.org/licenses/.
#
#  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
#############################################################################

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
