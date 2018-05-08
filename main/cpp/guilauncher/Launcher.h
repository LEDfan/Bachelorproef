//
// Created by niels on 5/8/18.
//

#ifndef STRIDE_LAUNCHMODEL_H
#define STRIDE_LAUNCHMODEL_H


#include <QtCore/QObject>

/**
 * Represents the current set data for the launch. Config files etc.
 */
class Launcher : public QObject{
    Q_OBJECT;

public:
    Launcher(): m_configPath() {}

    Q_INVOKABLE
    void setConfigPath(QString string);

    Q_INVOKABLE
    void launch();

private:
    std::string m_configPath;
};


#endif //STRIDE_LAUNCHMODEL_H
