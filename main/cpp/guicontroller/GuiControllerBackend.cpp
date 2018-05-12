#include "GuiControllerBackend.h"
#include <sim/Sim.h>

GuiControllerBackend::GuiControllerBackend(std::shared_ptr<stride::SimRunner> runner) {
    m_runner = runner;
}

void GuiControllerBackend::stepDay() {
    m_runner->GetSim()->TimeStep();
}
