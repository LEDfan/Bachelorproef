#pragma once

#include <QtCore/QObject>
#include <sim/SimRunner.h>

class GuiControllerBackend : public QObject
{
        Q_OBJECT
public:
        GuiControllerBackend(std::shared_ptr<stride::SimRunner> runner);

        Q_INVOKABLE
        void StepDay();
        Q_INVOKABLE
        unsigned int GetInfectedCount();
        Q_INVOKABLE
        unsigned int GetDay();

private:
        std::shared_ptr<stride::SimRunner> m_runner;
        unsigned int                       m_day;
};
