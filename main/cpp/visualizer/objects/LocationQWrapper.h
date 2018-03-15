#pragma once

#include <QObject>
#include <gengeopop/Location.h>

class LocationQWrapper : public QObject
{
public:
        std::shared_ptr<gengeopop::Location> location = nullptr;
};
