#pragma once

#include <QObject>
#include <QString>
#include <gengeopop/GeoGrid.h>
#include <set>

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
        void OnMarkerClicked(int idOfClicked);

        Q_INVOKABLE
        /**
         * Adds the location of the marker to the selection
         * @param idOfClicked
         */
        void OnExtraMarkerClicked(int idOfClicked);

        Q_INVOKABLE
        /**
         * Removes all locations from selection.
         */
        void clearSelection();

        Q_INVOKABLE
        void selectArea(double slat, double slong, double elat, double elong);

        Q_INVOKABLE
        /**
         *  Saves the current GeoGrid to a JSON file.
         * @param fileLoc File to save the JSON to.
         */
        void SaveGeoGridToFile(const QString& fileLoc, QObject* errorDialog);

signals:
        void LocationsSelected(std::set<std::shared_ptr<gengeopop::Location>> locations);

private:
        QObject*                                       m_map = nullptr;
        std::shared_ptr<gengeopop::GeoGrid>            m_grid;
        std::set<std::shared_ptr<gengeopop::Location>> m_selection; ///< The currently selected locations

        void PlaceMarker(Coordinate coordinate, std::string id, unsigned int population, bool selected);

        /*
         * Places the markers on the map, according to the current checked boxes.
         * @Pre: m_map is initialized correctly and holds the map we want to place markers on
         */
        void PlaceMarkers();

        /**
         * Sends a signal witht the currently selected locations.
         */
        void emitLocations();

        /**
         * Adds the location to the selection if it is not yet in the list.
         */
        void addToSelectionIfNoDuplicate(std::shared_ptr<gengeopop::Location> loc);
};
