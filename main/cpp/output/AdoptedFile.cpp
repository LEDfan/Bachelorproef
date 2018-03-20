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

#include "AdoptedFile.h"
#include "util/FileSys.h"

namespace stride {
namespace output {

using namespace std;
using namespace stride::util;

AdoptedFile::AdoptedFile(const string& output_prefix) : m_fstream() { Initialize(output_prefix); }

AdoptedFile::~AdoptedFile() { m_fstream.close(); }

void AdoptedFile::Print(const vector<unsigned int>& adopters)
{
        for (unsigned int i = 0; i < (adopters.size() - 1); i++) {
                m_fstream << adopters[i] << ",";
        }
        m_fstream << adopters[adopters.size() - 1] << std::endl;
}

void AdoptedFile::Initialize(const string& output_prefix)
{
        const auto p = FileSys::BuildPath(output_prefix, "adopted.csv");
        m_fstream.open(p.c_str());
}

} // namespace output
} // namespace stride
