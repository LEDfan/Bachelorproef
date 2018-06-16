#include "backend.h"

#include <QtCore/QFileInfo>
#include <QtCore/QPointF>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtCore/qdebug.h>
#include <QtQml/QQmlProperty>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <cmath>
#include <iostream>

#include <gengeopop/College.h>
#include <gengeopop/K12School.h>
#include <gengeopop/Workplace.h>
#include <gengeopop/io/GeoGridProtoReader.h>
#include <gengeopop/io/GeoGridReaderFactory.h>
#include <gengeopop/io/GeoGridWriterFactory.h>
#include <util/Stopwatch.h>

Backend::Backend(QObject* parent)
    : QObject(parent), m_grids(), m_markers(), m_commutes(), m_selection(), m_unselection()
{
}

Backend::Backend(const Backend&) : QObject(), m_grids(), m_markers(), m_commutes(), m_selection(), m_unselection() {}

Backend& Backend::operator=(const Backend& b)
{
        m_grids     = b.m_grids;
        m_selection = b.m_selection;
        return *this;
}

void Backend::LoadGeoGridFromFile(const QString& file, QObject* errorDialog)
{
        QUrl                                      info(file);
        std::string                               filename = info.toLocalFile().toStdString();
        gengeopop::GeoGridReaderFactory           factory;
        std::shared_ptr<gengeopop::GeoGridReader> reader = factory.CreateReader(filename);
        try {
                SetGeoGrids({reader->Read()});
        } catch (const std::exception& e) {
                QMetaObject::invokeMethod(errorDialog, "open");
                QQmlProperty(errorDialog, "text").write(QString("Error: ") + e.what());
        }
}

void Backend::SetGeoGrids(std::vector<std::shared_ptr<gengeopop::GeoGrid>> grids)
{
        m_grids = grids;
        for (auto grid : m_grids) {
                grid->Finalize();
        }
        m_selection.clear();
        m_unselection.clear();
        m_commutes.clear();
        PlaceMarkers();
}

void Backend::LoadGeoGridFromCommandLine(const QStringList& args)
{
        for (int i = 1; i < args.size(); i++) {
                boost::filesystem::path path(args[i].toStdString());
                if (boost::filesystem::exists(path)) {
                        path = boost::filesystem::canonical(path);
                        qDebug() << "Reading from " << path.c_str();

                        gengeopop::GeoGridReaderFactory           geoGridReaderFactory;
                        std::shared_ptr<gengeopop::GeoGridReader> reader =
                            geoGridReaderFactory.CreateReader(path.c_str());

                        try {
                                m_grids.push_back(reader->Read());
                                m_grids[0]->Finalize();
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
        if (m_showCommutes) {
                for (const auto& locID : m_selection) {
                        auto loc = GetLocationInRegion(locID);
                        for (auto commute : loc->GetIncomingCommuningCities()) {
                                auto otherCity = commute.first;
                                AddCommuteLine(otherCity->GetCoordinate(), loc->GetCoordinate(), commute.second);
                        }
                }
        }

        // Place the new markers
        int i = 0;
        for (auto grid : m_grids) {
                for (const std::shared_ptr<gengeopop::Location>& loc : *grid) {
                        bool         selected = m_selection.find({i, loc->GetID()}) != m_selection.end();
                        bool         special  = !loc->GetSubMunicipalities().empty();
                        unsigned int population =
                            special ? loc->GetPopulationOfSubmunicipalities() : loc->GetPopulation();
                        PlaceMarker(loc->GetCoordinate(), i, loc->GetID(), population, selected, special);
                }
                i++;
        }
}

void Backend::OnMarkerClicked(int region, unsigned int idOfClicked)
{
        auto loc = m_grids[region]->GetById(idOfClicked);

        ClearSelection();
        ToggleSelectionOfLocation(region, loc);

        EmitLocations();
        UpdateColorOfMarkers();
}

void Backend::SetObjects(QObject* map)
{
        m_map = map;
        PlaceMarkers();
}

void Backend::PlaceMarker(Coordinate coordinate, int region, int id, unsigned int population, bool selected,
                          bool specialmarker)
{
        QVariant returnVal;
        double   size = std::min(50.0, 10 + population * 0.0015);
        QMetaObject::invokeMethod(m_map, "addMarker", Qt::QueuedConnection, Q_ARG(QVariant, coordinate.latitude),
                                  Q_ARG(QVariant, coordinate.longitude), Q_ARG(QVariant, region), Q_ARG(QVariant, id),
                                  Q_ARG(QVariant, size), Q_ARG(QVariant, selected), Q_ARG(QVariant, specialmarker));
        m_markers[{region, id}] = qvariant_cast<QObject*>(returnVal);
}

void Backend::SaveGeoGridToFile(const QString& fileLoc, QObject* errorDialog)
{
        try {
                QUrl                            info(fileLoc);
                std::string                     filename = info.toLocalFile().toStdString();
                gengeopop::GeoGridWriterFactory geoGridWriterFactory;
                for (const auto& grid : m_grids) {
                        boost::filesystem::path filePath = filename;
                        filePath /= boost::filesystem::path(std::string("grid_region_") + grid->GetRegionName() +
                                                            std::string(".proto"));
                        std::shared_ptr<gengeopop::GeoGridWriter> writer =
                            geoGridWriterFactory.CreateWriter(filePath.string());
                        std::ofstream outputFile(filePath.string());
                        writer->Write(grid, outputFile);
                        outputFile.close();
                }
        } catch (const std::exception& e) {
                QMetaObject::invokeMethod(errorDialog, "open");
                QQmlProperty(errorDialog, "text").write(QString("Error: ") + e.what());
        }
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
        for (std::pair<int, int> locIDs : m_selection) {
                auto* marker = m_markers[locIDs];
                QMetaObject::invokeMethod(marker, "setBorder", Qt::DirectConnection, Q_ARG(QVariant, "purple"));
        }
        for (const auto& locIDs : m_unselection) {
                auto* marker = m_markers[locIDs];
                QMetaObject::invokeMethod(marker, "setBorder", Qt::DirectConnection, Q_ARG(QVariant, "black"));
        }
        m_selection.clear();
        m_unselection.clear();

        EmitLocations();
        UpdateColorOfMarkers();
}

void Backend::EmitLocations()
{
        std::set<std::shared_ptr<gengeopop::Location>> m_TempSelection;
        for (auto s : m_selection) {
                m_TempSelection.insert(GetLocationInRegion(s));
        }
        emit LocationsSelected(m_TempSelection);
}

void Backend::OnExtraMarkerClicked(int region, unsigned int idOfClicked)
{
        try {
                const auto& loc = m_grids[region]->GetById(idOfClicked);
                ToggleSelectionOfLocation(region, loc);

        } catch (...) {
        }
        EmitLocations();
        UpdateColorOfMarkers();
}

void Backend::ToggleSelectionOfLocation(int region, std::shared_ptr<gengeopop::Location> loc)
{
        const auto& subMun = loc->GetSubMunicipalities();
        if (m_selection.find(std::pair<int, int>(region, loc->GetID())) == m_selection.end()) {
                m_selection.emplace(region, loc->GetID());
                // Add subminicipalities
                for (const auto& mun : subMun) {
                        m_selection.insert({region, mun->GetID()});
                }
                m_unselection.erase({region, loc->GetID()});
        } else {
                m_selection.erase({region, loc->GetID()});
                // Add subminicipalities
                for (const auto& mun : subMun) {
                        m_selection.erase({region, mun->GetID()});
                        m_unselection.emplace(region, mun->GetID());
                }
                m_unselection.emplace(region, loc->GetID());
        }
}

void Backend::SelectArea(double slat, double slong, double elat, double elong)
{
        ClearSelection();
        SelectExtraInArea(slat, slong, elat, elong);
}

void Backend::SelectExtraInArea(double slat, double slong, double elat, double elong)
{
        m_unselection.clear();
        auto previousSelection = m_selection;
        try {
                int i = 0;
                for (auto& grid : m_grids) {
                        std::set<std::shared_ptr<gengeopop::Location>> m_gridSselectionOriginal =
                            grid->InBox(slong, slat, elong, elat);
                        std::set<std::pair<int, int>> m_gridSelection;
                        for (auto e : m_gridSselectionOriginal) {
                                m_gridSelection.insert(std::pair<int, int>{i, e->GetID()});
                        }
                        std::set_union(m_gridSelection.begin(), m_gridSelection.end(), m_selection.begin(),
                                       m_selection.end(), std::inserter(m_selection, m_selection.end()));
                        i++;
                }
                std::set_difference(previousSelection.begin(), previousSelection.end(), m_selection.begin(),
                                    m_selection.end(), std::inserter(m_unselection, m_unselection.end()));
        } catch (...) {
                // Can happen when geogrid is not yet loaded
                return;
        }

        EmitLocations();
        UpdateColorOfMarkers();
}

void Backend::UpdateColorOfMarkers()
{
        for (const auto& locID : m_unselection) {
                auto* marker = m_markers[locID];
                auto  loc    = GetLocationInRegion(locID);
                QMetaObject::invokeMethod(marker, "setBorder", Qt::DirectConnection, Q_ARG(QVariant, "black"));
                // Hide all connections between unselection and unselection, and selection and unselection
                if (m_showCommutes) {
                        for (const auto& otherLoc : *(m_grids[locID.first])) {
                                HideCommuteBetween(locID.first, loc, otherLoc);
                        }
                }
        }
        m_unselection.clear();
        for (const auto& locID : m_selection) {
                auto* marker = m_markers[locID];
                QMetaObject::invokeMethod(marker, "setBorder", Qt::DirectConnection, Q_ARG(QVariant, "purple"));
                // Show the commutes
                auto loc = GetLocationInRegion(locID);
                if (m_showCommutes) {
                        for (const auto& commute : loc->GetOutgoingCommuningCities()) {
                                // If the other city is also selected
                                if (m_selection.find({locID.first, commute.first->GetID()}) != m_selection.end()) {
                                        ShowCommute(locID.first, loc, commute.first);
                                }
                        }
                }
        }
}

QObject* Backend::AddCommuteLine(Coordinate from, Coordinate to, double /* amount */)
{
        QVariant retVal;
        QMetaObject::invokeMethod(m_map, "addCommute", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retVal),
                                  Q_ARG(QVariant, from.latitude), Q_ARG(QVariant, from.longitude),
                                  Q_ARG(QVariant, to.latitude), Q_ARG(QVariant, to.longitude));
        return qvariant_cast<QObject*>(retVal);
}

void Backend::SelectAll()
{
        int i = 0;
        for (auto grid : m_grids) {
                for (const auto& it : *grid) {
                        m_selection.emplace(i, it->GetID());
                }
                i++;
        }

        EmitLocations();
        PlaceMarkers();
}

void Backend::HideCommuteLine(QObject* line)
{
        QVariant retVal;
        QMetaObject::invokeMethod(line, "hide", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retVal));
}

void Backend::SetShowCommutes(bool value)
{
        m_showCommutes = value;
        // If we hide the commutes delete the current ones
        if (!value) {
                for (auto& commuteLine : m_commutes) {
                        // Hide and delete
                        HideCommuteLine(commuteLine.second);
                }
                m_commutes.clear();
        }
        ClearSelection();
}

void Backend::HideCommuteBetween(int region, const std::shared_ptr<gengeopop::Location>& loc1,
                                 const std::shared_ptr<gengeopop::Location>& loc2)
{
        std::tuple<int, unsigned int, unsigned int> key(region, loc1->GetID(), loc2->GetID());
        if (m_commutes.find(key) != m_commutes.end()) {
                QObject* commuteLine = m_commutes.find(key)->second;
                HideCommuteLine(commuteLine);
        }

        std::tuple<int, unsigned int, unsigned int> keyReversed(region, loc2->GetID(), loc1->GetID());
        if (m_commutes.find(keyReversed) != m_commutes.end()) {
                QObject* commuteLine = m_commutes.find(keyReversed)->second;
                HideCommuteLine(commuteLine);
        }
}

void Backend::ShowCommute(int region, const std::shared_ptr<gengeopop::Location>& loc1,
                          const std::shared_ptr<gengeopop::Location>& loc2)
{
        QVariant                                    retVal;
        QObject*                                    commuteLine = nullptr;
        std::tuple<int, unsigned int, unsigned int> key(region, loc1->GetID(), loc2->GetID());
        if (m_commutes.find(key) != m_commutes.end()) {
                commuteLine = m_commutes.find(key)->second;
                QMetaObject::invokeMethod(commuteLine, "show", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retVal));
        } else {
                // Does not yet exist
                commuteLine     = AddCommuteLine(loc1->GetCoordinate(), loc2->GetCoordinate(), 100);
                m_commutes[key] = commuteLine;
        }
}

void Backend::SaveMarker(int region, int id, QObject* marker) { m_markers[{region, id}] = marker; }

void Backend::UpdateAllHealthColors()
{
        int i = 0;
        for (auto grid : m_grids) {
                for (auto loc : *grid) {
                        SetHealthColorOf(i, loc);
                }
                i++;
        }
}

void Backend::SetHealthColorOf(int region, const std::shared_ptr<gengeopop::Location>& loc)
{
        auto*  marker        = m_markers[{region, loc->GetID()}];
        double infectedRatio = loc->GetInfectedRatio();

        // Check if it is a submuncipality
        if (loc->GetSubMunicipalities().size() > 0) {
                infectedRatio = loc->GetInfectedRatioOfSubmunicipalities();
        }

        double colorRatio = std::pow(infectedRatio, m_colorExponent);
        colorRatio        = std::max(0.0, colorRatio);
        colorRatio        = std::min(1.0, colorRatio);

        std::string color = (boost::format("#%02x%02x00") % static_cast<int>(colorRatio * 255) %
                             static_cast<int>((1 - colorRatio) * 255))
                                .str();

        if (color.length() != 7) {
                std::cout << "Wrong color " << color << std::endl;
                return;
        }

        QMetaObject::invokeMethod(marker, "setColor", Qt::QueuedConnection,
                                  Q_ARG(QVariant, QString::fromStdString(color)));
}

void Backend::OnMarkerHovered(int region, unsigned int idOfHover)
{
        auto loc = m_grids[region]->GetById(idOfHover);

        // Check if not in selection

        if (m_selection.find({region, idOfHover}) == m_selection.end()) {
                QObject* marker = m_markers[{region, loc->GetID()}];
                QMetaObject::invokeMethod(marker, "setBorder", Qt::DirectConnection, Q_ARG(QVariant, "blue"));

                // Change colors of submunicipalities
                const std::set<std::shared_ptr<gengeopop::Location>> sub = loc->GetSubMunicipalities();
                for (const auto& mun : sub) {
                        QObject* markerMun = m_markers[{region, mun->GetID()}];
                        QMetaObject::invokeMethod(markerMun, "setBorder", Qt::DirectConnection,
                                                  Q_ARG(QVariant, "blue"));
                }
        }
}

void Backend::OnMarkerHoveredOff(int region, unsigned int idOfHover)
{
        auto loc = m_grids[region]->GetById(idOfHover);
        // Check if not in selection
        if (m_selection.find({region, idOfHover}) == m_selection.end()) {
                QObject* locMarker = m_markers[{region, loc->GetID()}];
                QMetaObject::invokeMethod(locMarker, "setBorder", Qt::DirectConnection, Q_ARG(QVariant, "black"));

                // Change colors of submunicipalities
                const std::set<std::shared_ptr<gengeopop::Location>> sub = loc->GetSubMunicipalities();
                for (const auto& mun : sub) {
                        QObject* marker = m_markers[{region, mun->GetID()}];
                        if (m_selection.find({region, mun->GetID()}) == m_selection.end()) {
                                QMetaObject::invokeMethod(marker, "setBorder", Qt::DirectConnection,
                                                          Q_ARG(QVariant, "black"));
                        } else {
                                // Back to selection color
                                QMetaObject::invokeMethod(marker, "setBorder", Qt::DirectConnection,
                                                          Q_ARG(QVariant, "purple"));
                        }
                }
        }
}

std::shared_ptr<gengeopop::Location> Backend::GetLocationInRegion(std::pair<int, int> ids)
{
        return m_grids[ids.first]->GetById(ids.second);
}
