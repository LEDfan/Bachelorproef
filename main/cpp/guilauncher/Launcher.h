#pragma once
#include <QtCore/QObject>
#include <boost/property_tree/ptree.hpp>

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
        void setToLaunch();

        // Launch if the gui set that we should launch after it closed
        void launchIfSet();

        void SetRootObject(QObject* rootObject);

private:
        std::string m_configPath;
        bool        m_showVisualizer;
        bool        m_showMapViewer;
        bool        m_showAdoptedViewer;
        bool        m_showCliViewer;
        bool        m_showInfectedViewer;
        bool        m_showPersonsViewer;
        bool        m_showSummaryViewer;
        bool        m_setToLaunch = false; // Whether or not it is actually enabled to launch
        int         m_controller = 0;           //< The controller type we use. 0 = Cli, 1 = gui

        boost::property_tree::ptree m_configPt;

        QObject* m_rootObject;

        struct {
                QObject* r0 = nullptr;
        } m_configEditor;

        void UpdateConfigForm();
};

