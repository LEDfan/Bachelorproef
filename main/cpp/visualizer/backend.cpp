#include "backend.h"

#include <QtCore/QFileInfo>
#include <QtCore/QPointF>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtCore/qdebug.h>
#include <QtQml/QQmlProperty>

#include <boost/filesystem.hpp>

#include <cmath>
#include <iostream>

#include <gengeopop/HighSchool.h>
#include <gengeopop/School.h>
#include <gengeopop/Workplace.h>
#include <gengeopop/io/GeoGridJSONReader.h>
#include <gengeopop/io/GeoGridJSONWriter.h>
#include <util/Stopwatch.h>

Backend::Backend(QObject* parent)
    : QObject(parent), m_grid(std::make_shared<gengeopop::GeoGrid>()), m_markers(), m_commutes(), m_selection(),
      m_unselection()
{
}

Backend::Backend(const Backend&)
    : QObject(), m_grid(std::make_shared<gengeopop::GeoGrid>()), m_markers(), m_commutes(), m_selection(),
      m_unselection()
{
}

Backend& Backend::operator=(const Backend& b)
{
        m_grid      = b.m_grid;
        m_selection = b.m_selection;
        return *this;
}

void Backend::LoadGeoGridFromFile(const QString& file, QObject* errorDialog)
{
        QUrl                         info(file);
        std::ifstream                inputFile(info.toLocalFile().toStdString());
        gengeopop::GeoGridJSONReader reader;
        try {
                m_grid = reader.read(inputFile);
                m_grid->finalize();
        } catch (const std::exception& e) {
                QMetaObject::invokeMethod(errorDialog, "open");
                QQmlProperty(errorDialog, "text").write(QString("Error: ") + e.what());
        }
        PlaceMarkers();
}

void Backend::LoadGeoGridFromCommandLine(const QStringList& args)
{
        for (int i = 1; i < args.size(); i++) {
                boost::filesystem::path path(args[i].toStdString());
                if (boost::filesystem::exists(path)) {
                        path = boost::filesystem::canonical(path);
                        qDebug() << "Reading from " << path.c_str();

                        std::ifstream                inputFile(path.c_str());
                        gengeopop::GeoGridJSONReader reader;

                        try {
                                m_grid = reader.read(inputFile);
                                m_grid->finalize();
                        } catch (const std::exception& e) {
                                qWarning() << QString("Error: ") + e.what();
                        }
                        PlaceMarkers();
                        break;
                }
        }
}

void Backend::PlaceMarkers()
{
        // Clear the present markers
        m_commutes.clear();
        QMetaObject::invokeMethod(m_map, "clearMap");

        // Place the commutes of the selection
        for (const auto& loc : m_selection) {
                for (auto commute : loc->getIncomingCommuningCities()) {
                        auto otherCity = commute.first;
                        addCommuteLine(otherCity->getCoordinate(), loc->getCoordinate(), commute.second);
                }
        }

        // Place the new markers
        for (const std::shared_ptr<gengeopop::Location>& loc : *m_grid) {
                bool         selected   = m_selection.find(loc) != m_selection.end();
                bool         special    = !loc->getSubMunicipalities().empty();
                unsigned int population = special ? loc->getPopulationOfSubmunicipalities() : loc->getPopulation();
                PlaceMarker(loc->getCoordinate(), std::to_string(loc->getID()), population, selected, special);
        }
}

void Backend::OnMarkerClicked(unsigned int idOfClicked)
{
        auto loc = m_grid->GetById(idOfClicked);

        ClearSelection();
        toggleSelectionOfLocation(loc);

        emitLocations();
        UpdateColorOfMarkers();
}

void Backend::SetObjects(QObject* map)
{
        m_map = map;
        PlaceMarkers();
}

void Backend::PlaceMarker(Coordinate coordinate, std::string id, unsigned int population, bool selected,
                          bool specialmarker)
{
        QVariant returnVal;
        double   size = std::min(50.0, 10 + population * 0.0015);
        QMetaObject::invokeMethod(m_map, "addMarker", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnVal),
                                  Q_ARG(QVariant, coordinate.latitude), Q_ARG(QVariant, coordinate.longitude),
                                  Q_ARG(QVariant, QString(id.c_str())), Q_ARG(QVariant, size),
                                  Q_ARG(QVariant, selected), Q_ARG(QVariant, specialmarker));
        m_markers[id] = qvariant_cast<QObject*>(returnVal);
}

void Backend::SaveGeoGridToFile(const QString& fileLoc, QObject* errorDialog)
{
        QUrl                         info(fileLoc);
        std::ofstream                outputFile(info.toLocalFile().toStdString());
        gengeopop::GeoGridJSONWriter writer;
        try {
                writer.write(m_grid, outputFile);
        } catch (const std::exception& e) {
                QMetaObject::invokeMethod(errorDialog, "open");
                QQmlProperty(errorDialog, "text").write(QString("Error: ") + e.what());
        }
        outputFile.close();
}

void Backend::ClearSelection()
{
        m_unselection.clear();
        m_unselection.insert(m_selection.begin(), m_selection.end());
        m_selection.clear();
        UpdateColorOfMarkers();
        m_unselection.clear();
}

void Backend::ClearSelectionAndRender()
{
        for (const std::shared_ptr<gengeopop::Location>& loc : m_selection) {
                auto* marker = m_markers[std::to_string(loc->getID())]->findChild<QObject*>("rect");
                marker->setProperty("color", "red");
        }
        for (const std::shared_ptr<gengeopop::Location>& loc : m_unselection) {
                auto* marker = m_markers[std::to_string(loc->getID())]->findChild<QObject*>("rect");
                marker->setProperty("color", "red");
        }
        m_selection.clear();
        m_unselection.clear();

        emitLocations();
        UpdateColorOfMarkers();
}

void Backend::emitLocations() { emit LocationsSelected(m_selection); }

void Backend::OnExtraMarkerClicked(unsigned int idOfClicked)
{
        const auto& loc = m_grid->GetById(idOfClicked);
        toggleSelectionOfLocation(loc);

        emitLocations();
        UpdateColorOfMarkers();
}

void Backend::toggleSelectionOfLocation(std::shared_ptr<gengeopop::Location> loc)
{
        const auto& subMun = loc->getSubMunicipalities();
        if (m_selection.find(loc) == m_selection.end()) {
                m_selection.emplace(std::move(loc));
                // Add subminicipalities
                for (const auto& mun : subMun) {
                        m_selection.insert(mun);
                }
                m_unselection.erase(loc);
        } else {
                m_selection.erase(loc);
                // Add subminicipalities
                for (const auto& mun : subMun) {
                        m_selection.erase(mun);
                        m_unselection.emplace(mun);
                }
                m_unselection.emplace(std::move(loc));
        }
}

void Backend::selectArea(double slat, double slong, double elat, double elong)
{
        try {
                m_unselection.clear();
                std::set<std::shared_ptr<gengeopop::Location>> previousSelection = m_selection;
                m_selection = m_grid->inBox(slong, slat, elong, elat);
                std::set_difference(previousSelection.begin(), previousSelection.end(), m_selection.begin(),
                                    m_selection.end(), std::inserter(m_unselection, m_unselection.end()));
        } catch (std::exception& e) {
                // Can happen when geogrid is not yet loaded
                std::cout << e.what() << std::endl;
                return;
        }

        emitLocations();
        UpdateColorOfMarkers();
}

void Backend::UpdateColorOfMarkers()
{
        for (const std::shared_ptr<gengeopop::Location>& loc : m_unselection) {
                auto* marker = m_markers[std::to_string(loc->getID())]->findChild<QObject*>("rect");
                marker->setProperty("color", "red");
                // Hide all connections between unselection and unselection, and selection and unselection
                if (m_showCommutes) {
                        for (const auto& otherLoc : *m_grid) {
                                hideCommuteBetween(loc, otherLoc);
                        }
                }
        }
        m_unselection.clear();
        for (const std::shared_ptr<gengeopop::Location>& loc : m_selection) {
                auto* marker = m_markers[std::to_string(loc->getID())]->findChild<QObject*>("rect");
                marker->setProperty("color", "blue");
                // Show the commutes
                if (m_showCommutes) {
                        for (const auto& commute : loc->getOutgoingCommuningCities()) {
                                // If the other city is also selected
                                if (m_selection.find(commute.first) != m_selection.end()) {
                                        showCommute(loc, commute.first);
                                }
                        }
                }
        }
}

QObject* Backend::addCommuteLine(Coordinate from, Coordinate to, double /* amount */)
{
        QVariant retVal;
        QMetaObject::invokeMethod(m_map, "addCommute", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retVal),
                                  Q_ARG(QVariant, from.latitude), Q_ARG(QVariant, from.longitude),
                                  Q_ARG(QVariant, to.latitude), Q_ARG(QVariant, to.longitude));
        return qvariant_cast<QObject*>(retVal);
}

void Backend::selectAll()
{
        for (const auto& it : *m_grid) {
                m_selection.emplace(it);
        }

        emitLocations();
        PlaceMarkers();
}

void Backend::hideCommuteLine(QObject* line)
{
        QVariant retVal;
        QMetaObject::invokeMethod(line, "hide", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retVal));
}

void Backend::setShowCommutes(bool value)
{
        m_showCommutes = value;
        // Re render
        ClearSelectionAndRender();
        PlaceMarkers();
}

void Backend::hideCommuteBetween(const std::shared_ptr<gengeopop::Location>& loc1,
                                 const std::shared_ptr<gengeopop::Location>& loc2)
{
        std::tuple<unsigned int, unsigned int> key(loc1->getID(), loc2->getID());
        if (m_commutes.find(key) != m_commutes.end()) {
                QObject* commuteLine = m_commutes.find(key)->second;
                hideCommuteLine(commuteLine);
        }

        std::tuple<unsigned int, unsigned int> keyReversed(loc2->getID(), loc1->getID());
        if (m_commutes.find(keyReversed) != m_commutes.end()) {
                QObject* commuteLine = m_commutes.find(keyReversed)->second;
                hideCommuteLine(commuteLine);
        }
}

void Backend::showCommute(const std::shared_ptr<gengeopop::Location>& loc1,
                          const std::shared_ptr<gengeopop::Location>& loc2)
{
        QVariant                               retVal;
        QObject*                               commuteLine = nullptr;
        std::tuple<unsigned int, unsigned int> key(loc1->getID(), loc2->getID());
        if (m_commutes.find(key) != m_commutes.end()) {
                commuteLine = m_commutes.find(key)->second;
                QMetaObject::invokeMethod(commuteLine, "show", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retVal));
        } else {
                // Does not yet exist
                commuteLine     = addCommuteLine(loc1->getCoordinate(), loc2->getCoordinate(), 100);
                m_commutes[key] = commuteLine;
        }
}
