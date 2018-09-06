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
