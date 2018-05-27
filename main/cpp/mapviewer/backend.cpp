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
    : QObject(parent), m_grids(), m_markers(), m_commutes(), m_selection(),
      m_unselection()
{
}

Backend::Backend(const Backend&)
    : QObject(), m_grids(), m_markers(), m_commutes(), m_selection(),
      m_unselection()
{
}

Backend& Backend::operator=(const Backend& b)
{
        m_grids      = b.m_grids;
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
        for(auto grid : m_grids){
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
                for (const auto& loc : m_selection) {
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
                bool         selected   = m_selection.find(loc) != m_selection.end();
                bool         special    = !loc->GetSubMunicipalities().empty();
                unsigned int population = special ? loc->GetPopulationOfSubmunicipalities() : loc->GetPopulation();
                PlaceMarker(loc->GetCoordinate(), i, std::to_string(loc->GetID()), population, selected, special);
            }
            i++;
        }

}

void Backend::OnMarkerClicked(int region, unsigned int idOfClicked)
{
    // TODO
    try {
            auto loc = m_grids[region]->GetById(idOfClicked);

            ClearSelection();
            ToggleSelectionOfLocation(loc);

            EmitLocations();
            UpdateColorOfMarkers();
    } catch(...){}
}

void Backend::SetObjects(QObject* map)
{
        m_map = map;
        PlaceMarkers();
}

void Backend::PlaceMarker(Coordinate coordinate, int region, std::string id, unsigned int population, bool selected,
                          bool specialmarker)
{
        QVariant returnVal;
        double   size = std::min(50.0, 10 + population * 0.0015);
        QMetaObject::invokeMethod(m_map, "addMarker", Qt::QueuedConnection, Q_ARG(QVariant, coordinate.latitude),
                                  Q_ARG(QVariant, coordinate.longitude), Q_ARG(QVariant, region), Q_ARG(QVariant, QString(id.c_str())),
                                  Q_ARG(QVariant, size), Q_ARG(QVariant, selected), Q_ARG(QVariant, specialmarker));
        m_markers[id] = qvariant_cast<QObject*>(returnVal);
}

void Backend::SaveGeoGridToFile(const QString& fileLoc, QObject* errorDialog)
{
        QUrl                                      info(fileLoc);
        std::string                               filename = info.toLocalFile().toStdString();
        std::ofstream                             outputFile(filename);
        gengeopop::GeoGridWriterFactory           geoGridWriterFactory;
        std::shared_ptr<gengeopop::GeoGridWriter> writer = geoGridWriterFactory.CreateWriter(filename);
        try {
//                writer->Write(m_grid, outputFile); // TODO
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
                auto* marker = m_markers[std::to_string(loc->GetID())];
                QMetaObject::invokeMethod(marker, "setBorder", Qt::DirectConnection, Q_ARG(QVariant, "purple"));
        }
        for (const std::shared_ptr<gengeopop::Location>& loc : m_unselection) {
                auto* marker = m_markers[std::to_string(loc->GetID())];
                QMetaObject::invokeMethod(marker, "setBorder", Qt::DirectConnection, Q_ARG(QVariant, "black"));
        }
        m_selection.clear();
        m_unselection.clear();

        EmitLocations();
        UpdateColorOfMarkers();
}

void Backend::EmitLocations() { emit LocationsSelected(m_selection); }

void Backend::OnExtraMarkerClicked(int region, unsigned int idOfClicked)
{
        try{
                const auto& loc = m_grids[region]->GetById(idOfClicked);
                ToggleSelectionOfLocation(loc);

        }
        catch(...){}
        EmitLocations();
        UpdateColorOfMarkers();
}

void Backend::ToggleSelectionOfLocation(std::shared_ptr<gengeopop::Location> loc)
{
        const auto& subMun = loc->GetSubMunicipalities();
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

void Backend::SelectArea(double slat, double slong, double elat, double elong)
{
        m_unselection.clear();
        std::set<std::shared_ptr<gengeopop::Location>> previousSelection = m_selection;
        try {
                for (auto grid : m_grids) {
                        std::set<std::shared_ptr<gengeopop::Location> > m_gridSselection = grid->InBox(slong, slat, elong, elat);
                        std::set_union(m_gridSselection.begin(), m_gridSselection.end(), m_selection.begin(), m_selection.end(), std::inserter(m_selection, m_selection.end()));
                }
                std::set_difference(previousSelection.begin(), previousSelection.end(), m_selection.begin(),
                                    m_selection.end(), std::inserter(m_unselection, m_unselection.end()));
        } catch (std::exception &e) {
                // Can happen when geogrid is not yet loaded
                std::cout << e.what() << std::endl;
                return;
        }

        EmitLocations();
        UpdateColorOfMarkers();
}

void Backend::UpdateColorOfMarkers()
{
        for (const std::shared_ptr<gengeopop::Location>& loc : m_unselection) {
                auto* marker = m_markers[std::to_string(loc->GetID())];
                QMetaObject::invokeMethod(marker, "setBorder", Qt::DirectConnection, Q_ARG(QVariant, "black"));
                // Hide all connections between unselection and unselection, and selection and unselection
                if (m_showCommutes) {
//                        for (const auto& otherLoc : *m_grid) {
//                                HideCommuteBetween(loc, otherLoc);
//                        }
                }
        }
        m_unselection.clear();
        for (const std::shared_ptr<gengeopop::Location>& loc : m_selection) {
                auto* marker = m_markers[std::to_string(loc->GetID())];
                QMetaObject::invokeMethod(marker, "setBorder", Qt::DirectConnection, Q_ARG(QVariant, "purple"));
                // Show the commutes
                if (m_showCommutes) {
                        for (const auto& commute : loc->GetOutgoingCommuningCities()) {
                                // If the other city is also selected
                                if (m_selection.find(commute.first) != m_selection.end()) {
                                        ShowCommute(loc, commute.first);
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
        for (auto grid : m_grids){
            for (const auto& it : *grid) {
                m_selection.emplace(it);
            }
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
        // Re render
        ClearSelectionAndRender();
        PlaceMarkers();
}

void Backend::HideCommuteBetween(const std::shared_ptr<gengeopop::Location>& loc1,
                                 const std::shared_ptr<gengeopop::Location>& loc2)
{
        std::tuple<unsigned int, unsigned int> key(loc1->GetID(), loc2->GetID());
        if (m_commutes.find(key) != m_commutes.end()) {
                QObject* commuteLine = m_commutes.find(key)->second;
                HideCommuteLine(commuteLine);
        }

        std::tuple<unsigned int, unsigned int> keyReversed(loc2->GetID(), loc1->GetID());
        if (m_commutes.find(keyReversed) != m_commutes.end()) {
                QObject* commuteLine = m_commutes.find(keyReversed)->second;
                HideCommuteLine(commuteLine);
        }
}

void Backend::ShowCommute(const std::shared_ptr<gengeopop::Location>& loc1,
                          const std::shared_ptr<gengeopop::Location>& loc2)
{
        QVariant                               retVal;
        QObject*                               commuteLine = nullptr;
        std::tuple<unsigned int, unsigned int> key(loc1->GetID(), loc2->GetID());
        if (m_commutes.find(key) != m_commutes.end()) {
                commuteLine = m_commutes.find(key)->second;
                QMetaObject::invokeMethod(commuteLine, "show", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retVal));
        } else {
                // Does not yet exist
                commuteLine     = AddCommuteLine(loc1->GetCoordinate(), loc2->GetCoordinate(), 100);
                m_commutes[key] = commuteLine;
        }
}

void Backend::SaveMarker(QString id, QObject* marker) { m_markers[id.toStdString()] = marker; }

void Backend::UpdateAllHealthColors()
{
    for(auto grid: m_grids){
         for (auto loc : *grid) {
            SetHealthColorOf(loc); }
    }
}

void Backend::SetHealthColorOf(const std::shared_ptr<gengeopop::Location>& loc)
{
        auto*  marker        = m_markers[std::to_string(loc->GetID())];
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
        try{
        auto loc = m_grids[region]->GetById(idOfHover);


        // Check if not in selection

        if (m_selection.find(loc) == m_selection.end()) {
                QObject* marker = m_markers[std::to_string(loc->GetID())];
                QMetaObject::invokeMethod(marker, "setBorder", Qt::DirectConnection, Q_ARG(QVariant, "blue"));

                // Change colors of submunicipalities
                const std::set<std::shared_ptr<gengeopop::Location>> sub = loc->GetSubMunicipalities();
                for (const auto& mun : sub) {
                        QObject* markerMun = m_markers[std::to_string(mun->GetID())];
                        QMetaObject::invokeMethod(markerMun, "setBorder", Qt::DirectConnection,
                                                  Q_ARG(QVariant, "blue"));
                }
        }
                }
        catch(...){}
}

void Backend::OnMarkerHoveredOff(int region, unsigned int idOfHover)
{
    try{
        auto loc = m_grids[region]->GetById(idOfHover);
        // Check if not in selection
        if (m_selection.find(loc) == m_selection.end()) {
                QObject* locMarker = m_markers[std::to_string(loc->GetID())];
                QMetaObject::invokeMethod(locMarker, "setBorder", Qt::DirectConnection, Q_ARG(QVariant, "black"));

                // Change colors of submunicipalities
                const std::set<std::shared_ptr<gengeopop::Location>> sub = loc->GetSubMunicipalities();
                for (const auto& mun : sub) {
                        // Save the old color
                        QObject* marker = m_markers[std::to_string(mun->GetID())];
                        if (m_selection.find(mun) == m_selection.end()) {
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
        catch(std::exception e){
            std::cout << e.what() << std::endl;
    }
}
