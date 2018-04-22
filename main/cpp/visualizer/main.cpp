#include "Visualizer.h"
#include "backend.h"
#include "backends/LocationViewerBackend.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <gengeopop/io/GeoGridReaderFactory.h>
#include <thread>
#include <visualizer/backends/ContactCenterViewerBackend.h>
#include <visualizer/models/ContactCenterListModel.h>
#include <visualizer/models/ContactPoolListModel.h>

int main(int argc, char* argv[])
{
        Visualizer v;
        v.join();
}
