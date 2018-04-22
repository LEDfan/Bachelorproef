#pragma once

#include <QObject>
#include <QString>
#include <gengeopop/GeoGrid.h>
#include <set>
#include <unordered_map>

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
        void SetGeoGrid(std::shared_ptr<gengeopop::GeoGrid> grid);

        Q_INVOKABLE
        void saveMarker(QString id, QObject* marker);

        Q_INVOKABLE
        /**
         * Load a GeoGrid from JSON file, specified in the command line arguments
         */
        void LoadGeoGridFromCommandLine(const QStringList& args);

        Q_INVOKABLE
        /**
         * Places the locations of the current GeoGrid on the map.
         * @param map: Instance of the Map QObject
         */
        void SetObjects(QObject* map);

        Q_INVOKABLE
        /**
         * Handles a click on a marker. Will emit a locationSelected signal with the correct location
         * that correspons to the clicked marker.
         * @param idOfClicked The id of the marker that was clicked.
         */
        void OnMarkerClicked(unsigned int idOfClicked);

        Q_INVOKABLE
        /**
         * Adds the location of the marker to the selection
         * @param idOfClicked
         */
        void OnExtraMarkerClicked(unsigned int idOfClicked);

        Q_INVOKABLE
        /**
         * Handles the action that is needed when a marker is hovered over.
         * @param idOfHover id of marker we hover over
         */
        void onMarkerHovered(unsigned int idOfHover);
        Q_INVOKABLE
        /**
         * Handles the action that is needed when a marker is no longer hovered over.
         * @param idOfHover id of marker we hovered over
         */
        void onMarkerHoveredOff(unsigned int idOfHover);

        Q_INVOKABLE
        /**
         * Removes all locations from selection and unselection + re-render the map.
         */
        void ClearSelectionAndRender();

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
        void SelectAll();

        Q_INVOKABLE
        /**
         * @param value If commutes need to be shown
         */
        void SetShowCommutes(bool value);

        Q_INVOKABLE
        void updateAllHealthColors();

        Q_INVOKABLE
        /**
         *  Saves the current GeoGrid to a JSON file.
         * @param fileLoc File to save the JSON to.
         */
        void SaveGeoGridToFile(const QString& fileLoc, QObject* errorDialog);

signals:
        /// Emits the locations that are selected. Other components can connect to this to be notified.
        void LocationsSelected(std::set<std::shared_ptr<gengeopop::Location>> locations);

private:
        QObject*                            m_map = nullptr; ///< The QML Map the info is displayed on
        std::shared_ptr<gengeopop::GeoGrid> m_grid;
        std::map<std::string, QObject*>     m_markers; ///< Reference to the markers so we do not need to search
        std::map<std::tuple<unsigned int, unsigned int>, QObject*>
                                                       m_commutes; ///< The commute lines that are shown on the map, KEY is the id of the city the commutes go to
        bool                                           m_showCommutes = false;
        std::set<std::shared_ptr<gengeopop::Location>> m_selection; ///< The currently selected locations
        std::set<std::shared_ptr<gengeopop::Location>>
            m_unselection; ///< Items which must be unselected until the next UpdateColorOfMarkres call

        /**
         * Places a marker at the given coordinate
         * @Param specialmarker Whether or not to display a special marker
         */
        void PlaceMarker(Coordinate coordinate, std::string id, unsigned int population, bool selected,
                         bool specialmarker);

        /**
         * Places the markers on the map, according to the current checked boxes.
         * @Pre: m_map is initialized correctly and holds the map we want to place markers on
         */
        void PlaceMarkers();

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
        void ToggleSelectionOfLocation(std::shared_ptr<gengeopop::Location> loc);

        /**
         * Add a line on the map for the given commute info.
         * @param fromLatitude
         * @param fromLongitude
         * @param toLatitude
         * @param toLongitude
         */
        QObject* AddCommuteLine(Coordinate from, Coordinate to, double amount);

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
        void HideCommuteBetween(const std::shared_ptr<gengeopop::Location>& loc1,
                                const std::shared_ptr<gengeopop::Location>& loc2);

        /**
         * Shows the commute from loc1 to loc2 on the map.
         * @param loc1
         */
        void ShowCommute(const std::shared_ptr<gengeopop::Location>& loc1, const std::shared_ptr<gengeopop::Location>&);
};
