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
        void showLocation(std::shared_ptr<gengeopop::Location> location);

signals:
        void contactCenterSelected();

private:
        QObject* _parent = nullptr;
};
