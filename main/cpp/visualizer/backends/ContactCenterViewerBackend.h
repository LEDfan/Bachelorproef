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
        void showContactCenter(std::shared_ptr<gengeopop::ContactCenter> location);

private:
        QObject* _parent = nullptr;
};
