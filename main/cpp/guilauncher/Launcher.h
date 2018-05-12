//
// Created by niels on 5/8/18.
//

#ifndef STRIDE_LAUNCHMODEL_H
#define STRIDE_LAUNCHMODEL_H

#include <QtCore/QObject>

/**
 * Represents the current set data for the launch. Config files etc.
 */
class Launcher : public QObject
{
        Q_OBJECT;

public:
        Launcher() : m_configPath(), m_showVisualizer(false) {}

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
};

#endif // STRIDE_LAUNCHMODEL_H
