#pragma once

#include <QObject>
#include <QString>
#include <gengeopop/GeoGrid.h>

class LocationViewerBackend : public QObject
{
        Q_OBJECT

public:
        explicit LocationViewerBackend(QObject* parent = nullptr);

        Q_INVOKABLE
        /**
         * Adapts the text and tableView model to show the correct info about the location.
         * @param location The location to show in the viewer.
         */
        void showLocations(std::vector<std::shared_ptr<gengeopop::Location>> location);

signals:
        void contactCenterSelected();
};
