#include <QtCore/QThread>
#include <iostream>

#include "GuiControllerBackend.h"
#include "SimWorker.h"
#include <sim/Sim.h>

GuiControllerBackend::GuiControllerBackend() : m_runner(nullptr), m_thread(std::make_shared<QThread>()), m_day(0) {}

void GuiControllerBackend::SetRunner(std::shared_ptr<stride::SimRunner> runner) { m_runner = runner; }

void GuiControllerBackend::StepDay()
{
        if (!m_runner)
                return;
        m_thread->quit();
        m_thread->wait();
        stride::SimWorker* simWorker = new stride::SimWorker(m_runner);
        simWorker->moveToThread(m_thread.get());
        connect(m_thread.get(), &QThread::started, simWorker, &stride::SimWorker::Work, Qt::DirectConnection);
        connect(simWorker, &stride::SimWorker::Worked, this, &GuiControllerBackend::UpdateViews, Qt::DirectConnection);
        connect(m_thread.get(), &QThread::finished, simWorker, &stride::SimWorker::deleteLater, Qt::DirectConnection);
        m_thread->start();

        m_day++;
}

unsigned int GuiControllerBackend::GetInfectedCount()
{
        if (!m_runner)
                return 0;
        return m_runner->GetSim()->GetPopulation()->GetInfectedCount();
}
unsigned int GuiControllerBackend::GetDay() { return m_day; }

void GuiControllerBackend::UpdateViews(unsigned int infectedCount) { emit Stepped(infectedCount, m_day); }
GuiControllerBackend::~GuiControllerBackend()
{
        m_thread->quit();
        m_thread->wait();
}
