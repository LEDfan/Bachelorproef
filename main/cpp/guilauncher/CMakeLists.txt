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
    set(GUILAUNCHER_SRC
        Launcher.cpp
        Launcher.h
    )

    qt5_add_resources(GUILAUNCHER_QT_RESOURCES launcherqml.qrc)

    #============================================================================
    # Build and link the guilauncher executable.
    #============================================================================
    add_library(libguilauncher ${GUILAUNCHER_SRC} ${GUILAUNCHER_QT_RESOURCES})
    target_compile_options(libguilauncher PUBLIC "-fPIC")

    unset(GUILAUNCHER_QT_RESOURCES)
    unset(GUILAUNCHER_SRC)
endif()

