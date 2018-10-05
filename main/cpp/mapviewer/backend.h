/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <gengeopop/GeoGrid.h>
#include <set>
#include <unordered_map>

/**
 * The backend for the MapViewer qml
 */
class Backend : public QObject
{
        Q_OBJECT

public:
        explicit Backend(QObject* parent = nullptr);
        Backend(const Backend& b);
        Backend& operator=(const Backend& b);

        Q_INVOKABLE
        /**
         * Loads a GeoGrid from JSON file.
         * @param file The path to the JSON file that contains a valid GeoGrid description.
         */
        void LoadGeoGridFromFile(const QString& file, QObject* errorDialog);

        Q_INVOKABLE
        /**
         * Changes the geogrid to the given one. Redraws the markers on the map.
         * @param grid The grid we want to change to.
         */
        void SetGeoGrids(std::vector<std::shared_ptr<gengeopop::GeoGrid>> grids);

        Q_INVOKABLE
        /**
         * Load a GeoGrid from JSON file, specified in the command line arguments
         */
        void LoadGeoGridFromCommandLine(const QStringList& args);
        Q_INVOKABLE
        /**
         * Handles a click on a marker. Will emit a locationSelected signal with the correct location
         * that correspons to the clicked marker.
         * @param idOfClicked The id of the marker that was clicked.
         */
        void OnMarkerClicked(int region, unsigned int idOfClicked);

        Q_INVOKABLE
        /**
         * Adds the location of the marker to the selection
         * @param idOfClicked
         */
        void OnExtraMarkerClicked(int region, unsigned int idOfClicked);

        Q_INVOKABLE
        /**
         * Handles the action that is needed when a marker is hovered over.
         * @param idOfHover id of marker we hover over
         */
        void OnMarkerHovered(int region, unsigned int idOfHover);
        Q_INVOKABLE
        /**
         * Handles the action that is needed when a marker is no longer hovered over.
         * @param idOfHover id of marker we hovered over
         */
        void OnMarkerHoveredOff(int region, unsigned int idOfHover);

        Q_INVOKABLE
        /**
         * Removes all locations from selection and unselection but don't re-render the map.
         */
        void ClearSelection();

        Q_INVOKABLE
        /**
         * Selects all locations in between
         * @param slat
         * @param slong
         * @param elat
         * @param elong
         */
        void SelectArea(double slat, double slong, double elat, double elong);

        Q_INVOKABLE
        /**
         * The Same as SelectArea but does not clear selection first.
         */
        void SelectExtraInArea(double slat, double slong, double elat, double elong);

        Q_INVOKABLE
        /**
         * Adds all locations of the grid to the selection and updates the visual appearance of these locations.
         */
        void SelectAll();

        Q_INVOKABLE
        /**
         * @param value If commutes need to be shown
         */
        void SetShowCommutes(bool value);

        Q_INVOKABLE
        /**
         * Updates all the health colors of the locations to match the current infection rate.
         */
        void UpdateAllHealthColors();

        Q_INVOKABLE
        /**
         * Places the locations of the current GeoGrid on the map.
         * @param map: Instance of the Map QObject
         */
        void SetObjects(QObject* map);

        Q_INVOKABLE
        /**
         *  Saves the current GeoGrid to a JSON file.
         * @param fileLoc File to save the JSON to.
         */
        void SaveGeoGridToFile(const QString& fileLoc, QObject* errorDialog);

        Q_INVOKABLE
        /**
         * Saves the CustomMarker QObject of a specific location so we can modify it later.
         * @param region the region of the location
         * @param id the id of the location
         */
        void SaveMarker(int region, int id, QObject* marker);

signals:
        /// Emits the locations that are selected. Other components can connect to this to be notified.
        void LocationsSelected(std::set<std::shared_ptr<gengeopop::Location>> locations);

        /// Emits a signal that the infected count should be updated
        void UpdateInfected();

private:
        double   m_colorExponent = 0.1;     ///< We use this exponent to make the color change not linear
        QObject* m_map           = nullptr; ///< The QML Map the info is displayed on
        std::vector<std::shared_ptr<gengeopop::GeoGrid>> m_grids;
        std::map<std::tuple<int, int>, QObject*> m_markers; ///< Reference to the markers so we do not need to search
        /// The first entry of the tuple is the region, second entry is the id of the location
        /// The corresponding marker is the marker of that location if it is currently on the map

        std::map<std::tuple<int, unsigned int, unsigned int>, QObject*>
                                      m_commutes; ///< The commute lines that are shown on the map, KEY is the id of the city the commutes go to
        bool                          m_showCommutes = false;
        std::set<std::pair<int, int>> m_selection; ///< The currently selected locations (id of region, id of location)
        std::set<std::pair<int, int>>
                                            m_unselection; ///< Items which must be unselected until the next UpdateColorOfMarkres call
        std::shared_ptr<stride::Population> m_population; ///< Population used when importing the GeoGrid from a file

        /**
         * Places a marker at the given coordinate
         */
        void PlaceMarker(gengeopop::Coordinate coordinate, int region, int id, unsigned int population, bool selected);

        /**
         * Places the markers on the map, according to the current checked boxes.
         * @Pre: m_map is initialized correctly and holds the map we want to place markers on
         */
        void PlaceMarkers();

        Q_INVOKABLE
        /**
         * Removes all locations from selection and unselection + re-render the map.
         */
        void ClearSelectionAndRender();

        /**
         * Update colors of Markers based on the value of m_selection and m_unselection.
         * Won't loop over every marker or location.
         */
        void UpdateColorOfMarkers();

        /**
         * Sends a signal with the currently selected locations.
         */
        void EmitLocations();

        /**
         * Adds the location to the selection if it is not yet in the list. If it is already in the selection, it will
         * be removed.
         */
        void ToggleSelectionOfLocation(int region, std::shared_ptr<gengeopop::Location> loc);

        /**
         * Add a line on the map for the given commute info.
         * @param fromLatitude
         * @param fromLongitude
         * @param toLatitude
         * @param toLongitude
         */
        QObject* AddCommuteLine(gengeopop::Coordinate from, gengeopop::Coordinate to, double amount);

        /**
         * Hides the commute line on the map.
         * @param obj The commute line QObject
         */
        void HideCommuteLine(QObject* obj);

        /**
         * Hides the commutes between the given 2 locations on the map.
         * @param loc1
         * @param loc2
         */
        void HideCommuteBetween(int region, const std::shared_ptr<gengeopop::Location>& loc1,
                                const std::shared_ptr<gengeopop::Location>& loc2);

        /**
         * Shows the commute from loc1 to loc2 on the map.
         * @param loc1
         */
        void ShowCommute(int region, const gengeopop::Location* loc1, const gengeopop::Location*);

        /**
         * Updates the health color of the location on the map so it represents the current situation.
         * @param region the id of the region the location is in
         * @param loc The location we want to update on the map.
         */
        void SetHealthColorOf(int region, const std::shared_ptr<gengeopop::Location>& loc);

        /**
         * Return the requested location
         * @param ids (region id, location id)
         */
        std::shared_ptr<gengeopop::Location> GetLocationInRegion(std::pair<int, int> ids);
};
