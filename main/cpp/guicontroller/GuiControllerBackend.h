#pragma once

#include <QtCore/QObject>

#include <sim/SimRunner.h>

class GuiControllerBackend : public QObject
{
        Q_OBJECT
public:
        GuiControllerBackend();
        ~GuiControllerBackend();

        void SetRunner(std::shared_ptr<stride::SimRunner> runner);

        Q_INVOKABLE
        void StepDay();
        Q_INVOKABLE
        unsigned int GetInfectedCount();
        Q_INVOKABLE
        unsigned int GetDay();

private slots:
        void UpdateViews(unsigned int infectedCount);

signals:
        void Stepped(unsigned int infectedCount, unsigned int day);

private:
        std::shared_ptr<stride::SimRunner> m_runner;
        std::shared_ptr<QThread>           m_thread;
        unsigned int                       m_day;
};
