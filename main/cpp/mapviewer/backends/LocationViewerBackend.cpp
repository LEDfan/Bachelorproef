/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

#include "LocationViewerBackend.h"
#include <QTableView>
#include <QtCore/QVariant>
#include <iostream>

LocationViewerBackend::LocationViewerBackend(QObject* parent) : QObject(parent), m_locations() {}

void LocationViewerBackend::ShowLocations(std::set<std::shared_ptr<gengeopop::Location>> locations)
{
        m_locations         = locations;
        auto nameText       = parent()->findChild<QObject*>(QString("textName"));
        auto provinceText   = parent()->findChild<QObject*>(QString("textProvince"));
        auto idText         = parent()->findChild<QObject*>(QString("textID"));
        auto populationText = parent()->findChild<QObject*>(QString("textPopulation"));
        auto infectedText   = parent()->findChild<QObject*>(QString("textInfected"));

        QString nameString("Name: ");
        QString provinceString("Province: ");
        QString idString("ID: ");
        QString populationString("Population: ");
        QString infectedString("Infected: ");

        int maxChars = 30;

        bool first = true;
        for (const auto& location : locations) {
                // Handle situation when too much is selected to be shown fully
                QString provinceName = NISToProvince(location->GetProvince());
                if (nameString.length() + QString::fromStdString(location->GetName()).length() > maxChars ||
                    provinceString.length() + provinceName.length() > maxChars ||
                    idString.length() + QString::number(location->GetID()).length() > maxChars) {
                        nameString += "...";
                        provinceString += "...";
                        idString += "...";
                        break;
                }

                if (first) {
                        first = false;
                } else {
                        nameString += QString(",");
                        provinceString += QString(",");
                        idString += QString(",");
                }

                nameString += QString::fromStdString(location->GetName());
                provinceString += provinceName;
                idString += QString::number(location->GetID());
        }

        // Calculate aggregated population
        unsigned int totalPopulation = 0;
        unsigned int totalInfected   = 0;
        for (const auto& location : locations) {
                totalPopulation += location->GetSimulationPopulation();
                totalInfected += location->GetInfectedCount();
        }
        populationString += QString::number(totalPopulation);
        infectedString += QString::number(totalInfected);

        nameText->setProperty("text", nameString);
        provinceText->setProperty("text", provinceString);
        idText->setProperty("text", idString);
        populationText->setProperty("text", populationString);
        infectedText->setProperty("text", infectedString);
}

void LocationViewerBackend::UpdateInfected() { ShowLocations(m_locations); }

QString LocationViewerBackend::NISToProvince(unsigned int id)
{
        switch (id) {
        case 1: return "Antwerpen";
        case 2: return "Vlaams-Brabant";
        case 3: return "West-Vlaanderen";
        case 4: return "Oost-Vlaanderen";
        case 7: return "Limburg";
        default: return "Onbekend";
        }
}
