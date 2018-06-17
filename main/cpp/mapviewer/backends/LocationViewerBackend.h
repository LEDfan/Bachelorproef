#pragma once

#include <QObject>
#include <QString>
#include <gengeopop/GeoGrid.h>
#include <set>

/**
 * This backend sets the correct info in the GUI of the location that is being shown.
 */
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
        void ShowLocations(std::set<std::shared_ptr<gengeopop::Location>> location);

signals:
        void ContactCenterSelected();

private:
        /**
         * @param id
         * @return the name of the Province that coressponds with the given NIS id
         */
        QString NISToProvince(unsigned int id);
};
