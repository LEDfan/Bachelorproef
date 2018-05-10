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
        void launch(bool showMapViewer, bool showAdoptedViewer, bool showCliViewer, bool showInfectedViewer,
                    bool showPersonsViewer, bool showSummaryViewer);

private:
        std::string m_configPath;
        bool        m_showVisualizer;
};

#endif // STRIDE_LAUNCHMODEL_H
