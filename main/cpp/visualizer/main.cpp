#include "backend.h"
#include "backends/LocationViewerBackend.h"
#include "Visualizer.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <visualizer/backends/ContactCenterViewerBackend.h>
#include <visualizer/models/ContactCenterListModel.h>
#include <visualizer/models/ContactPoolListModel.h>
#include <thread>

int main(int argc, char* argv[])
{

    auto grid = std::make_shared<gengeopop::GeoGrid>();
    Visualizer v;
    v.setGeoGrid(grid);
}
