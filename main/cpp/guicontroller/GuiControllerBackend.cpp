#include <iostream>

#include "GuiControllerBackend.h"
#include <sim/Sim.h>

GuiControllerBackend::GuiControllerBackend(std::shared_ptr<stride::SimRunner> runner) : m_runner(runner), m_day(0) {}

void GuiControllerBackend::StepDay()
{
        m_runner->Run(1);
        m_day++;
}

unsigned int GuiControllerBackend::GetInfectedCount()
{
        return m_runner->GetSim()->GetPopulation()->GetInfectedCount();
}
unsigned int GuiControllerBackend::GetDay() { return m_day; }
