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
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

#include <boost/property_tree/xml_parser.hpp>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <iostream>

#include "Launcher.h"
#include <guicontroller/GuiController.h>
#include <mapviewer/MapViewer.h>
#include <sim/BaseController.h>
#include <sim/CliController.h>
#include <util/FileSys.h>
#include <util/TimeStamp.h>
#include <viewers/AdoptedFileViewer.h>
#include <viewers/CliViewer.h>
#include <viewers/InfectedViewer.h>
#include <viewers/PersonsFileViewer.h>
#include <viewers/SummaryFileViewer.h>

Launcher::Launcher() : m_configPath(), m_configPt(), m_configEditor() {}

void Launcher::Launch()
{
        // Only launch if we have set it to.
        if (!m_setToLaunch) {
                return;
        }

        // -----------------------------------------------------------------------------------------
        // Get configuration and path with overrides (if any).
        // -----------------------------------------------------------------------------------------
        //        configPt = stride::util::FileSys::ReadPtreeFile(m_configPath);
        if (m_configPt.get<std::string>("run.output_prefix", "").empty()) {
                m_configPt.put<std::string>("run.output_prefix", stride::util::TimeStamp().ToTag().append("/"));
        }

        m_configPt.sort();
        std::unique_ptr<stride::BaseController> controller = nullptr;
        QQmlApplicationEngine*                  engine     = nullptr;

        if (m_controller == 0) {
                auto guiController = std::make_unique<stride::GuiController>(m_configPt);
                engine             = guiController->GetEngine();
                controller         = std::move(guiController);
        } else {
                controller = std::make_unique<stride::CliController>(m_configPt);
        }

        std::unique_ptr<std::thread> thread = nullptr;

        if (m_showAdoptedViewer) {
                controller->RegisterViewer<stride::viewers::AdoptedFileViewer>(controller->GetOutputPrefix());
        }
        if (m_showCliViewer) {
                controller->RegisterViewer<stride::viewers::CliViewer>(controller->GetLogger());
        }
        if (m_showInfectedViewer) {
                controller->RegisterViewer<stride::viewers::InfectedViewer>();
        }
        if (m_showPersonsViewer) {
                controller->RegisterViewer<stride::viewers::PersonsFileViewer>(controller->GetOutputPrefix());
        }
        if (m_showSummaryViewer) {
                controller->RegisterViewer<stride::viewers::SummaryFileViewer>(controller->GetOutputPrefix());
        }
        if (m_showMapViewer) {
                if (!engine) {
                        Q_INIT_RESOURCE(qml);
                        int             i = 0;
                        QGuiApplication app(i, nullptr);
                        auto            localEngine = std::make_unique<QQmlApplicationEngine>();
                        controller->RegisterViewer<stride::viewers::MapViewer>(controller->GetLogger(),
                                                                               localEngine.get());
                        thread = std::make_unique<std::thread>([&controller]() { controller->Control(); });
                        app.exec();
                } else {
                        controller->RegisterViewer<stride::viewers::MapViewer>(controller->GetLogger(), engine);
                }
        }
        if (thread) {
                thread->join();
        } else {
                controller->Control();
        }
}

void Launcher::SetConfigPath(QString file)
{
        QUrl info(file);
        m_configPath = info.toLocalFile().toStdString();
        m_configPt   = stride::util::FileSys::ReadPtreeFile(m_configPath);
        UpdateConfigForm();
}

void Launcher::SetConfig(bool showMapViewer, bool showAdoptedViewer, bool showCliViewer, bool showInfectedViewer,
                         bool showPersonsViewer, bool showSummaryViewer)
{
        m_showMapViewer      = showMapViewer;
        m_showAdoptedViewer  = showAdoptedViewer;
        m_showCliViewer      = showCliViewer;
        m_showInfectedViewer = showInfectedViewer;
        m_showPersonsViewer  = showPersonsViewer;
        m_showSummaryViewer  = showSummaryViewer;
}

void Launcher::SetController(int index) { m_controller = index; }

void Launcher::UpdateConfigForm()
{
        m_configEditor.ageContactMatrixFile->setProperty(
            "text", m_configPt.get<std::string>("run.age_contact_matrix_file").c_str());

        LoadComboBox(m_configEditor.beliefPolicy, m_configPt.get<std::string>("run.belief_policy.name").c_str());

        LoadComboBox(m_configEditor.contactLogLevel, m_configPt.get<std::string>("run.contact_log_level").c_str());

        m_configEditor.diseaseConfigFile->setProperty("text",
                                                      m_configPt.get<std::string>("run.disease_config_file").c_str());

        LoadComboBox(m_configEditor.globalInformationPolicy,
                     m_configPt.get<std::string>("run.global_information_policy").c_str());
        m_configEditor.holidaysFile->setProperty("text", m_configPt.get<std::string>("run.holidays_file").c_str());

        LoadComboBox(m_configEditor.immunityProfile, m_configPt.get<std::string>("run.immunity_profile").c_str());

        m_configEditor.immunityRate->setProperty("value", m_configPt.get<std::string>("run.immunity_rate").c_str());

        LoadComboBox(m_configEditor.localInformationPolicy,
                     m_configPt.get<std::string>("run.local_information_policy").c_str());

        m_configEditor.numDays->setProperty("value", m_configPt.get<int>("run.num_days"));
        m_configEditor.numberParticipantsSurvey->setProperty("value",
                                                             m_configPt.get<int>("run.num_participants_survey"));
        m_configEditor.outputAdopted->setProperty("checked", m_configPt.get<bool>("run.output_adopted"));
        m_configEditor.outputCases->setProperty("checked", m_configPt.get<bool>("run.output_cases"));
        m_configEditor.outputPersons->setProperty("value", m_configPt.get<std::string>("run.output_persons").c_str());
        m_configEditor.outputPrefix->setProperty("text", m_configPt.get<std::string>("run.output_prefix").c_str());
        m_configEditor.outputSummary->setProperty("checked", m_configPt.get<bool>("run.output_summary"));
        m_configEditor.outputPersons->setProperty("checked", m_configPt.get<bool>("run.output_persons"));

        m_configEditor.numThreads->setProperty("value", m_configPt.get<std::string>("run.num_threads").c_str());
        m_configEditor.populationFile->setProperty("text", m_configPt.get<std::string>("run.population_file").c_str());
        m_configEditor.rngSeed->setProperty("value", m_configPt.get<std::string>("run.rng_seed").c_str());

        LoadComboBox(m_configEditor.rngType, m_configPt.get<std::string>("run.rng_type").c_str());

        m_configEditor.r0->setProperty("value", m_configPt.get<int>("run.r0"));
        m_configEditor.seedingAgeMax->setProperty("value", m_configPt.get<int>("run.seeding_age_max"));
        m_configEditor.seedingAgeMin->setProperty("value", m_configPt.get<int>("run.seeding_age_min"));
        m_configEditor.seedingRate->setProperty("value", m_configPt.get<double>("run.seeding_rate"));
        m_configEditor.startDate->setProperty("text", m_configPt.get<std::string>("run.start_date").c_str());

        LoadComboBox(m_configEditor.strideLogLevel, m_configPt.get<std::string>("run.stride_log_level").c_str());

        m_configEditor.trackIndexCase->setProperty("checked", m_configPt.get<bool>("run.track_index_case"));

        m_configEditor.vaccineLinkProbability->setProperty(
            "value", m_configPt.get<std::string>("run.vaccine_link_probability").c_str());

        LoadComboBox(m_configEditor.vaccineProfile, m_configPt.get<std::string>("run.vaccine_profile").c_str());

        m_configEditor.vaccineRate->setProperty("value", m_configPt.get<std::string>("run.vaccine_rate").c_str());

        // GeoPop
        std::string gengeopop_type;
        if (m_configPt.get_child("run").count("geopop_type")) {
                gengeopop_type = m_configPt.get<std::string>("run.geopop_type");
        } else {
                gengeopop_type = "multi-region";
        }
        LoadComboBox(m_configEditor.geopopType, gengeopop_type.c_str());
        if (gengeopop_type == "import") {
                m_configEditor.geopopFile->setProperty("text",
                                                       m_configPt.get<std::string>("run.geopop_import_file").c_str());
        } else if (gengeopop_type == "generate") {
                m_configEditor.geopopPopulationSize->setProperty(
                    "value", m_configPt.get<std::string>("run.geopop_gen.population_size").c_str());
                m_configEditor.geopopFraction1826Students->setProperty(
                    "value",
                    m_configPt.get<std::string>("run.geopop_gen.fraction_1826_years_which_are_students").c_str());
                m_configEditor.geopopFractionActiveCommuting->setProperty(
                    "value", m_configPt.get<std::string>("run.geopop_gen.fraction_active_commuting_people").c_str());
                m_configEditor.geopopFactionStudentCommuting->setProperty(
                    "value", m_configPt.get<std::string>("run.geopop_gen.fraction_student_commuting_people").c_str());
                m_configEditor.geopopFraction1865YearsActive->setProperty(
                    "value", m_configPt.get<std::string>("run.geopop_gen.fraction_1865_years_active").c_str());
                m_configEditor.geopopCities->setProperty(
                    "text", m_configPt.get<std::string>("run.geopop_gen.cities_file").c_str());
                m_configEditor.geopopCommuting->setProperty(
                    "text", m_configPt.get<std::string>("run.geopop_gen.commuting_file").c_str());
                m_configEditor.geopopHousehold->setProperty(
                    "text", m_configPt.get<std::string>("run.geopop_gen.household_file").c_str());
        }
}

void Launcher::UpdatePtree()
{
        m_configPt.put("run.age_contact_matrix_file",
                       m_configEditor.ageContactMatrixFile->property("text").toString().toStdString());

        m_configPt.put("run.belief_policy.name",
                       m_configEditor.beliefPolicy->property("currentText").toString().toStdString());

        m_configPt.put("run.contact_log_level",
                       m_configEditor.contactLogLevel->property("currentText").toString().toStdString());

        m_configPt.put("run.disease_config_file",
                       m_configEditor.diseaseConfigFile->property("text").toString().toStdString());

        m_configPt.put("run.global_information_policy",
                       m_configEditor.globalInformationPolicy->property("currentText").toString().toStdString());

        m_configPt.put("run.holidays_file", m_configEditor.holidaysFile->property("text").toString().toStdString());

        m_configPt.put("run.immunity_profile",
                       m_configEditor.immunityProfile->property("currentText").toString().toStdString());

        m_configPt.put("run.immunity_rate", m_configEditor.immunityRate->property("value").toString().toStdString());

        m_configPt.put("run.local_information_policy",
                       m_configEditor.localInformationPolicy->property("currentText").toString().toStdString());

        m_configPt.put("run.num_days", m_configEditor.numDays->property("value").toString().toStdString());

        m_configPt.put("run.num_participants_survey",
                       m_configEditor.numberParticipantsSurvey->property("value").toString().toStdString());

        m_configPt.put("run.output_adopted",
                       m_configEditor.outputAdopted->property("checked").toString().toStdString());

        m_configPt.put("run.output_cases", m_configEditor.outputCases->property("checked").toString().toStdString());

        m_configPt.put("run.output_persons",
                       m_configEditor.outputPersons->property("checked").toString().toStdString());

        m_configPt.put("run.output_prefix", m_configEditor.outputPrefix->property("text").toString().toStdString());

        m_configPt.put("run.output_summary",
                       m_configEditor.outputSummary->property("checked").toString().toStdString());

        m_configPt.put("run.output_persons",
                       m_configEditor.outputPersons->property("checked").toString().toStdString());

        m_configPt.put("run.num_threads", m_configEditor.numThreads->property("value").toString().toStdString());

        m_configPt.put("run.population_file", m_configEditor.populationFile->property("text").toString().toStdString());

        m_configPt.put("run.rng_seed", m_configEditor.rngSeed->property("value").toString().toStdString());

        m_configPt.put("run.rng_type", m_configEditor.rngType->property("currentText").toString().toStdString());

        m_configPt.put("run.r0", m_configEditor.r0->property("value").toString().toStdString());

        m_configPt.put("run.seeding_age_max", m_configEditor.seedingAgeMax->property("value").toString().toStdString());

        m_configPt.put("run.seeding_age_min", m_configEditor.seedingAgeMin->property("value").toString().toStdString());

        m_configPt.put("run.seeding_rate", m_configEditor.seedingRate->property("value").toString().toStdString());

        m_configPt.put("run.start_date", m_configEditor.startDate->property("text").toString().toStdString());

        m_configPt.put("run.stride_log_level",
                       m_configEditor.strideLogLevel->property("currentText").toString().toStdString());

        m_configPt.put("run.track_index_case",
                       m_configEditor.trackIndexCase->property("checked").toString().toStdString());

        m_configPt.put("run.vaccine_link_probability",
                       m_configEditor.vaccineLinkProbability->property("checked").toString().toStdString());

        m_configPt.put("run.vaccine_profile",
                       m_configEditor.vaccineProfile->property("checked").toString().toStdString());

        m_configPt.put("run.vaccine_rate", m_configEditor.vaccineRate->property("value").toString().toStdString());

        // GeoPop
        if (m_configPt.get_child("run").count("geopop_import_file")) {
                m_configPt.get_child("run").erase("geopop_import_file");
        }
        if (m_configPt.get_child("run").count("geopop_gen")) {
                m_configPt.get_child("run").erase("geopop_gen");
        }

        std::string gengeopop_type;
        if (m_configPt.get_child("run").count("geopop_type")) {
                gengeopop_type = m_configPt.get<std::string>("run.geopop_type");
                m_configPt.put("run.geopop_type", gengeopop_type);
        } else {
                std::string gengeopop_type = "multi-region";
        }
        if (gengeopop_type == "import") {
                m_configPt.put("run.geopop_import_file",
                               m_configEditor.geopopFile->property("text").toString().toStdString());
        } else if (gengeopop_type == "generate") {
                m_configPt.put("run.geopop_gen.population_size",
                               m_configEditor.geopopPopulationSize->property("value").toString().toStdString());
                m_configPt.put("run.geopop_gen.fraction_1826_years_which_are_students",
                               m_configEditor.geopopFraction1826Students->property("value").toString().toStdString());
                m_configPt.put(
                    "run.geopop_gen.fraction_active_commuting_people",
                    m_configEditor.geopopFractionActiveCommuting->property("value").toString().toStdString());
                m_configPt.put(
                    "run.geopop_gen.fraction_student_commuting_people",
                    m_configEditor.geopopFactionStudentCommuting->property("value").toString().toStdString());
                m_configPt.put(
                    "run.geopop_gen.fraction_1865_years_active",
                    m_configEditor.geopopFraction1865YearsActive->property("value").toString().toStdString());
                m_configPt.put("run.geopop_gen.cities_file",
                               m_configEditor.geopopCities->property("text").toString().toStdString());
                m_configPt.put("run.geopop_gen.commuting_file",
                               m_configEditor.geopopCommuting->property("text").toString().toStdString());
                m_configPt.put("run.geopop_gen.household_file",
                               m_configEditor.geopopHousehold->property("text").toString().toStdString());
        }
}

void Launcher::SetRootObject(QObject* rootObject)
{
        m_configEditor.ageContactMatrixFile       = rootObject->findChild<QObject*>("inputAgeContactMatrixFile");
        m_configEditor.behaviourPolicy            = rootObject->findChild<QObject*>("inputBehaviourPolicy");
        m_configEditor.beliefPolicy               = rootObject->findChild<QObject*>("inputBeliefPolicy");
        m_configEditor.contactLogLevel            = rootObject->findChild<QObject*>("inputContactLogLevel");
        m_configEditor.diseaseConfigFile          = rootObject->findChild<QObject*>("inputDiseaseConfigFile");
        m_configEditor.globalInformationPolicy    = rootObject->findChild<QObject*>("inputGlobalInformationPolicy");
        m_configEditor.holidaysFile               = rootObject->findChild<QObject*>("inputHolidaysFile");
        m_configEditor.immunityProfile            = rootObject->findChild<QObject*>("inputImmunityProfile");
        m_configEditor.immunityRate               = rootObject->findChild<QObject*>("inputImmunityRate");
        m_configEditor.localInformationPolicy     = rootObject->findChild<QObject*>("inputLocalInformationPolicy");
        m_configEditor.numDays                    = rootObject->findChild<QObject*>("inputNumDays");
        m_configEditor.numberParticipantsSurvey   = rootObject->findChild<QObject*>("inputNumberParticipantsSurvey");
        m_configEditor.outputAdopted              = rootObject->findChild<QObject*>("inputOutputAdopted");
        m_configEditor.outputCases                = rootObject->findChild<QObject*>("inputOutputCases");
        m_configEditor.outputPersons              = rootObject->findChild<QObject*>("inputOutputPersons");
        m_configEditor.outputPrefix               = rootObject->findChild<QObject*>("inputOutputPrefix");
        m_configEditor.outputPersons              = rootObject->findChild<QObject*>("inputOutputPersons");
        m_configEditor.outputSummary              = rootObject->findChild<QObject*>("inputOutputSummary");
        m_configEditor.numThreads                 = rootObject->findChild<QObject*>("inputNumThreads");
        m_configEditor.populationFile             = rootObject->findChild<QObject*>("inputPopulationFile");
        m_configEditor.rngSeed                    = rootObject->findChild<QObject*>("inputRngSeed");
        m_configEditor.rngType                    = rootObject->findChild<QObject*>("inputRngType");
        m_configEditor.r0                         = rootObject->findChild<QObject*>("inputR0");
        m_configEditor.seedingAgeMax              = rootObject->findChild<QObject*>("inputSeedingAgeMax");
        m_configEditor.seedingAgeMin              = rootObject->findChild<QObject*>("inputSeedingAgeMin");
        m_configEditor.seedingRate                = rootObject->findChild<QObject*>("inputSeedingRate");
        m_configEditor.startDate                  = rootObject->findChild<QObject*>("inputStartDate");
        m_configEditor.strideLogLevel             = rootObject->findChild<QObject*>("inputStrideLogLevel");
        m_configEditor.trackIndexCase             = rootObject->findChild<QObject*>("inputTrackIndexCase");
        m_configEditor.vaccineLinkProbability     = rootObject->findChild<QObject*>("inputVaccineLinkProbability");
        m_configEditor.vaccineProfile             = rootObject->findChild<QObject*>("inputVaccineProfile");
        m_configEditor.vaccineRate                = rootObject->findChild<QObject*>("inputVaccineRate");
        m_configEditor.geopopType                 = rootObject->findChild<QObject*>("gengeopopType");
        m_configEditor.geopopFile                 = rootObject->findChild<QObject*>("gengeopopFile");
        m_configEditor.geopopPopulationSize       = rootObject->findChild<QObject*>("gengeopopPopulationSize");
        m_configEditor.geopopFraction1826Students = rootObject->findChild<QObject*>("gengeopopFraction1826Students");
        m_configEditor.geopopFractionActiveCommuting =
            rootObject->findChild<QObject*>("gengeopopFractionActiveCommuting");
        m_configEditor.geopopFactionStudentCommuting =
            rootObject->findChild<QObject*>("gengeopopFractionStudentsCommuting");
        m_configEditor.geopopFraction1865YearsActive = rootObject->findChild<QObject*>("gengeopopFraction1856Active");
        m_configEditor.geopopCities                  = rootObject->findChild<QObject*>("gengeopopCitiesFile");
        m_configEditor.geopopCommuting               = rootObject->findChild<QObject*>("gengeopopCommutingFile");
        m_configEditor.geopopHousehold               = rootObject->findChild<QObject*>("gengeopopHouseholdFile");
}

void Launcher::LoadComboBox(QObject* comboBox, const char* value)
{
        QVariant retVal;

        QMetaObject::invokeMethod(comboBox, "find", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retVal),
                                  Q_ARG(QVariant, value));

        comboBox->setProperty("currentIndex", retVal);
}

void Launcher::SaveConfig(QString string)
{
        UpdatePtree();
        if (string.length() < 7)
                return;
        std::string filename = string.toStdString().substr(7, static_cast<int>(string.length()));
        stride::util::FileSys::WritePtreeFile(filename, m_configPt);
}

void Launcher::SetToLaunch() { m_setToLaunch = true; }
