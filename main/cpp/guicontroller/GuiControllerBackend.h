#pragma once

#include <QtCore/QObject>

#include <sim/SimRunner.h>
/**
 * A class used as a backend for the QML to provide simulation functionality
 */
class GuiControllerBackend : public QObject
{
        Q_OBJECT
public:
        /// Constructor
        GuiControllerBackend();
        ~GuiControllerBackend();
        /// Set the simulation runner. This is necessary for the GuiControllerBackend to operpat
        void SetRunner(std::shared_ptr<stride::SimRunner> runner);

        Q_INVOKABLE
        /// Step a day in the simulator, this will be done on a separate thread
        void StepDay();
        Q_INVOKABLE
        /// Query the simulation for the current infected count
        unsigned int GetInfectedCount();
        Q_INVOKABLE
        /// Get the current day in the simulation
        unsigned int GetDay();

private slots:
        /// Called by the SimWorker to notify the backend that a step in the simulation was completed
        void UpdateViews(unsigned int infectedCount);

signals:
        /// Signals QML components that a step in the simulation was completed
        void Stepped(unsigned int infectedCount, unsigned int day);

private:
        std::shared_ptr<stride::SimRunner> m_runner; ///< The simulation runner
        std::shared_ptr<QThread>           m_thread; ///< A thread for the SimWorker to run in
        unsigned int                       m_day;    ///< The current day in the simulation
};
