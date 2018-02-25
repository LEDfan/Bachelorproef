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
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Implementation of the CasesFile class.
 */

#include "PersonFile.h"
#include "pop/Population.h"
#include <boost/filesystem.hpp>

namespace stride {
namespace output {

using namespace std;
using namespace boost::filesystem;

PersonFile::PersonFile(const string& output_dir) { Initialize(output_dir); }

PersonFile::~PersonFile() { m_fstream.close(); }

void PersonFile::Initialize(const string& output_dir)
{
        path pathname(output_dir);
        pathname /= "person.csv";
        m_fstream.open(pathname.c_str());

        // add header
        m_fstream << "id,age,is_recovered,is_immune,start_infectiousness,"
                  << "end_infectiousness,start_symptomatic,end_symptomatic" << endl;
}

void PersonFile::Print(std::shared_ptr<const Population> population)
{
        for (const auto& p : *population) {
                const auto& h = p.GetHealth();
                m_fstream << p.GetId() << "," << p.GetAge() << "," << h.IsRecovered() << "," << h.IsImmune() << ","
                          << h.GetStartInfectiousness() << "," << h.GetEndInfectiousness() << ","
                          << h.GetStartSymptomatic() << "," << h.GetEndSymptomatic() << std::endl;
        }
}

} // namespace output
} // namespace stride
