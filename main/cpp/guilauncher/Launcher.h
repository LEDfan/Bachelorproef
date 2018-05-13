#pragma once
#include <boost/property_tree/ptree.hpp>
#include <QtCore/QObject>
#include <QtWidgets/QComboBox>

/**
 * Represents the current set data for the launch. Config files etc.
 */

class Launcher : public QObject
{
        Q_OBJECT

public:
        Launcher();

        Q_INVOKABLE
        void setConfigPath(QString string);

        Q_INVOKABLE
        void setConfig(bool showMapViewer, bool showAdoptedViewer, bool showCliViewer, bool showInfectedViewer,
                       bool showPersonsViewer, bool showSummaryViewer);

        Q_INVOKABLE
        /**
         * Sets the controller type we will use after launch
         * @param index 0 = Cli, 1 = GUI
         */
        void setController(int index);

        Q_INVOKABLE
        void launch();

        void SetRootObject(QObject* rootObject);

private:
        std::string m_configPath;
        bool        m_showVisualizer     = false;
        bool        m_showMapViewer      = false;
        bool        m_showAdoptedViewer  = false;
        bool        m_showCliViewer      = false;
        bool        m_showInfectedViewer = false;
        bool        m_showPersonsViewer  = false;
        bool        m_showSummaryViewer  = false;

        bool m_setToLaunch = false; // Whether or not it is actually enabled to launch
        int  m_controller  = 0;     //< The controller type we use. 0 = Cli, 1 = gui

        boost::property_tree::ptree m_configPt;

        QObject* m_rootObject = nullptr;

        struct
        {
                QObject* ageContactMatrixFile     = nullptr;
                QObject* behaviourPolicy          = nullptr;
                QObject* beliefPolicy             = nullptr;
                QObject* contactLogLevel          = nullptr;
                QObject* diseaseConfigFile        = nullptr;
                QObject* globalInformationPolicy  = nullptr;
                QObject* holidaysFile             = nullptr;
                QObject* immunityProfile          = nullptr;
                QObject* immunityRate             = nullptr;
                QObject* localInformationPolicy   = nullptr;
                QObject* numDays                  = nullptr;
                QObject* numberParticipantsSurvey = nullptr;
                QObject* outputAdopted            = nullptr;
                QObject* outputCases              = nullptr;
                QObject* outputPersons            = nullptr;
                QObject* outputPrefix             = nullptr;
                QObject* outputSummary            = nullptr;
                QObject* numThreads               = nullptr;
                QObject* populationFile           = nullptr;
                QObject* rngSeed                  = nullptr;
                QObject* rngType                  = nullptr;
                QObject* r0                       = nullptr;
                QObject* seedingAgeMax            = nullptr;
                QObject* seedingAgeMin            = nullptr;
                QObject* startDate                = nullptr;
                QObject* strideLogLevel           = nullptr;
                QObject* trackIndexCase           = nullptr;
                QObject* vaccineLinkProbability   = nullptr;
                QObject* vaccineProfile           = nullptr;
                QObject* vaccineRate              = nullptr;
        } m_configEditor;

        void UpdateConfigForm();

        void UpdatePtree();

        void LoadComboBox(QObject* comboBox, const char* value);
};
