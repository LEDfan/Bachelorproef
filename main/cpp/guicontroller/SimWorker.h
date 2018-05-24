#pragma once

#include <QtCore/QObject>
#include <memory>
#include <sim/SimRunner.h>

namespace stride {
/**
 * A worker class used to perform 1 step of the simulation on a different thread.
 */
class SimWorker : public QObject
{
        Q_OBJECT
public:
        /// Construct the Simworker with the SimRunner it has to perform a step for
        explicit SimWorker(std::shared_ptr<SimRunner> runner);
public slots:
        /// Perform the actual step if we are not at the end of the sim
        void Work();
signals:
        /// Let the caller know that the simulation was completely run and give the current infected count.
        void Worked(unsigned int infectedCount);

private:
        std::shared_ptr<SimRunner> m_runner; ///< The runner used to perform a step
};

} // namespace stride
