#include "SimWorker.h"
#include <sim/Sim.h>

namespace stride {

SimWorker::SimWorker(std::shared_ptr<SimRunner> runner) : m_runner(runner) {}

void SimWorker::Work()
{
        if (!m_runner->IsAtEndOfSim()) {
                m_runner->Run(1);
                emit Worked(m_runner->GetSim()->GetPopulation()->GetInfectedCount());
        }
}

} // namespace stride
