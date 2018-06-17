#include "LocationViewerBackend.h"
#include <QTableView>
#include <QtCore/QVariant>
#include <iostream>

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

LocationViewerBackend::LocationViewerBackend(QObject* parent) : QObject(parent) {}

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
