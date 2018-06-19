#pragma once
#include <boost/property_tree/ptree.hpp>
#include <QtCore/QObject>
#include <QtWidgets/QComboBox>

/**
 * Represents the current set data for the launch. Config files etc.
 * It also serves as a backend for loading and storing the configuration from xml files.
 */

class Launcher : public QObject
{
        Q_OBJECT

public:
        /// Create a Launcher
        Launcher();

        Launcher(const Launcher& b) = delete;
        Launcher& operator=(const Launcher& b) = delete;

        Q_INVOKABLE
        /**
         * Load the configuration from the provided path. This updates the ConfigForm.
         */
        void SetConfigPath(QString string);

        Q_INVOKABLE
        /**
         * Save the configuration stored in the ConfigForm to the provided filename.
         * It requires the string to start with `file://`.
         */
        void SaveConfig(QString string);

        Q_INVOKABLE
        /**
         * Set the enabled and disabled Viewers to be used in the simulation
         */
        void SetConfig(bool showMapViewer, bool showAdoptedViewer, bool showCliViewer, bool showInfectedViewer,
                       bool showPersonsViewer, bool showSummaryViewer);

        Q_INVOKABLE
        /**
         * Sets the controller type we will use after launch
         * @param index 0 = Cli, 1 = GUI
         */
        void SetController(int index);

        Q_INVOKABLE
        /**
         * Launch the simulation with the stored configuration.
         * This requires the QGuiApplication to already be closed
         */
        void Launch();

        /**
         * Set the Qt RootObject. This is needed to interact with the QML forms.
         * This should be set by the owner of this object after loading the QML.
         */
        void SetRootObject(QObject* rootObject);

        Q_INVOKABLE
        /**
         * Set if the Launcher has to actually launch or not.
         * If Launch() is called without calling this first, nothing happens.
         * This is to make sure the simulation is only launched when actually pressing the "Launch" button in the Gui
         * and not just closing the Launcher.
         */
        void SetToLaunch();

        Q_INVOKABLE
        /**
         * Updates the locally stored Boost Property Tree with the information in the QML forms.
         * This is called from the QML when changes are detected.
         */
        void UpdatePtree();

private:
        /**
         * Updates the information in the QML forms with what is stored in the Boost Property Tree.
         */
        void UpdateConfigForm();

        /**
         * Sets the value of the provided ComboBox to the provided value
         */
        void LoadComboBox(QObject* comboBox, const char* value);

        std::string m_configPath;                 ///< The path of the configuration file
        bool        m_showMapViewer      = false; ///< Show the MapViewer?
        bool        m_showAdoptedViewer  = false; ///< Show the AdoptedViewer?
        bool        m_showCliViewer      = false; ///< Show the CliViewer?
        bool        m_showInfectedViewer = false; ///< Show the InfectedViewer?
        bool        m_showPersonsViewer  = false; ///< Show the PersonsViewer?
        bool        m_showSummaryViewer  = false; ///< Show the SummaryViewer?

        int m_controller = 0; //< The controller type we use. 0 = Cli, 1 = gui

        boost::property_tree::ptree m_configPt; ///< The Property Tree loaded from m_configPath

        QObject* m_rootObject = nullptr; ///< The Gui RootObject, for querying QML objects

        bool m_setToLaunch = false; ///< Does the simulation have to launch?
        struct
        {
                QObject* ageContactMatrixFile          = nullptr;
                QObject* behaviourPolicy               = nullptr;
                QObject* beliefPolicy                  = nullptr;
                QObject* contactLogLevel               = nullptr;
                QObject* diseaseConfigFile             = nullptr;
                QObject* globalInformationPolicy       = nullptr;
                QObject* holidaysFile                  = nullptr;
                QObject* immunityProfile               = nullptr;
                QObject* immunityRate                  = nullptr;
                QObject* localInformationPolicy        = nullptr;
                QObject* numDays                       = nullptr;
                QObject* numberParticipantsSurvey      = nullptr;
                QObject* outputAdopted                 = nullptr;
                QObject* outputCases                   = nullptr;
                QObject* outputPersons                 = nullptr;
                QObject* outputPrefix                  = nullptr;
                QObject* outputSummary                 = nullptr;
                QObject* numThreads                    = nullptr;
                QObject* populationFile                = nullptr;
                QObject* rngSeed                       = nullptr;
                QObject* rngType                       = nullptr;
                QObject* r0                            = nullptr;
                QObject* seedingAgeMax                 = nullptr;
                QObject* seedingAgeMin                 = nullptr;
                QObject* seedingRate                   = nullptr;
                QObject* startDate                     = nullptr;
                QObject* strideLogLevel                = nullptr;
                QObject* trackIndexCase                = nullptr;
                QObject* vaccineLinkProbability        = nullptr;
                QObject* vaccineProfile                = nullptr;
                QObject* vaccineRate                   = nullptr;
                QObject* geopopType                    = nullptr;
                QObject* geopopFile                    = nullptr;
                QObject* geopopPopulationSize          = nullptr;
                QObject* geopopFraction1826Students    = nullptr;
                QObject* geopopFractionActiveCommuting = nullptr;
                QObject* geopopFactionStudentCommuting = nullptr;
                QObject* geopopFraction1865YearsActive = nullptr;
                QObject* geopopCities                  = nullptr;
                QObject* geopopCommuting               = nullptr;
                QObject* geopopHousehold               = nullptr;
                QObject* geopopSubmunicipalities       = nullptr;
        } m_configEditor; ///< The form objects in the QML that store the config
};
