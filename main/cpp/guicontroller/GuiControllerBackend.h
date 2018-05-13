#pragma once

#include <QtCore/QObject>
#include <sim/SimRunner.h>

class GuiControllerBackend : public QObject
{
        Q_OBJECT
public:
        GuiControllerBackend(std::shared_ptr<stride::SimRunner> runner);

        Q_INVOKABLE
        void stepDay();
        Q_INVOKABLE
        unsigned int getInfectedCount();
        Q_INVOKABLE
        unsigned int getDay();

private:
        std::shared_ptr<stride::SimRunner> m_runner;
        unsigned int                       m_day;
};
