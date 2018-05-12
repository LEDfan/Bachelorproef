#include "GuiControllerBackend.h"
#include <iostream>
#include <sim/Sim.h>

GuiControllerBackend::GuiControllerBackend(std::shared_ptr<stride::SimRunner> runner) : m_runner(runner) {}

void GuiControllerBackend::stepDay()
{
        m_runner->Run(1);
        m_day++;
}

unsigned int GuiControllerBackend::getInfectedCount()
{
        return m_runner->GetSim()->GetPopulation()->GetInfectedCount();
}
unsigned int GuiControllerBackend::getDay() { return m_day; }
