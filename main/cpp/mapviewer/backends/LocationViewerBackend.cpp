#include "LocationViewerBackend.h"
#include <QTableView>
#include <QtCore/QVariant>
#include <iostream>

void LocationViewerBackend::ShowLocations(std::set<std::shared_ptr<gengeopop::Location>> locations)
{
        auto nameText       = parent()->findChild<QObject*>(QString("textName"));
        auto provinceText   = parent()->findChild<QObject*>(QString("textProvince"));
        auto idText         = parent()->findChild<QObject*>(QString("textID"));
        auto populationText = parent()->findChild<QObject*>(QString("textPopulation"));

        QString nameString("Name: ");
        QString provinceString("Province: ");
        QString idString("ID: ");
        QString populationString("Population: ");

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
        for (const auto& location : locations) {
                totalPopulation += location->GetPopulation();
        }
        populationString += QString::number(totalPopulation);

        nameText->setProperty("text", nameString);
        provinceText->setProperty("text", provinceString);
        idText->setProperty("text", idString);
        populationText->setProperty("text", populationString);
}

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
