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
#include <gengeopop/io/GeoGridReaderFactory.h>

int main(int argc, char* argv[])
{

    auto grid = std::make_shared<gengeopop::GeoGrid>();
    Visualizer v;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    v.setGeoGrid(grid);
    v.join();
}
