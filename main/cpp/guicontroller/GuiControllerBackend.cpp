/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

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
