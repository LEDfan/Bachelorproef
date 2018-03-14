#pragma once

#include <QObject>
#include <QString>
#include <gengeopop/GeoGrid.h>

class LocationViewerBackEnd : public QObject
{
        Q_OBJECT

public:
        explicit LocationViewerBackEnd(QObject* parent = nullptr);

        Q_INVOKABLE
        void showLocation(const QObject& location);

signals:

        void contactCenterSelected();

private:
};
