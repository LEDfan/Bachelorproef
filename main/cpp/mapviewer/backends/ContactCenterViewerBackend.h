#pragma once

#include <QObject>
#include <QString>
#include <gengeopop/GeoGrid.h>

class ContactCenterViewerBackend : public QObject
{
        Q_OBJECT

public:
        explicit ContactCenterViewerBackend(QObject* parent = nullptr);

        Q_INVOKABLE
        /**
         * Changes the texts and table in the viewer to show the given contactCenter.
         * @param contactCenter The location to show in the viewer.
         */
        void ShowContactCenter(std::shared_ptr<gengeopop::ContactCenter> contactCenter);
};
