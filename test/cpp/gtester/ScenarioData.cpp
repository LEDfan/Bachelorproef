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
 *  Copyright 2018 Willem L, Kuylen E, Stijven S & Broeckhove J
 */

/**
 * @file
 * Implementation of scenario tests data.
 */

#include "ScenarioData.h"

#include "util/RunConfigPtree.h"

using namespace std;
using namespace stride;
using namespace stride::util;
using boost::property_tree::ptree;

namespace Tests {


tuple<ptree, unsigned int, double> ScenarioData::Get(const string& tag)
{
        ptree pt = tag.substr(0, 2) != "r0" ? RunConfigPtree::CreateTestsBasic1() : RunConfigPtree::CreateTestsBasic2();
        unsigned int target = 0U;
        double       sigma  = 0;

        if (tag == "influenza_a") {
                target = 1085U;
                sigma  = 116.60326645015648;
        }
        if (tag == "influenza_b") {
                pt.put("run.seeding_rate", 0.0);
                target = 0U;
                sigma  = 0;
        }
        if (tag == "influenza_c") {
                pt.put("run.seeding_rate", (1 - 0.9991) / 100);
                pt.put("run.immunity_rate", 0.9991);
                target = 5U;
                sigma  = 0;
        }
        if (tag == "measles_16") {
                pt.put("run.disease_config_file", "disease_measles.xml");
                pt.put("run.r0", 16U);
                target = 599900U;
                sigma  = 660.8227044636814;
        }
        if (tag == "measles_60") {
                pt.put("run.disease_config_file", "disease_measles.xml");
                pt.put("run.r0", 60U);
                target = 600000U;
                sigma  = 0;
        }

        // Run_r0 data
        if (tag == "r0_0") {
                pt.put("run.r0", 0.0);
                target = 1200U;
        }
        if (tag == "r0_4") {
                pt.put("run.r0", 4.0);
                target = 39080U;
        }
        if (tag == "r0_8") {
                pt.put("run.r0", 8.0);
                target = 110884U;
        }
        if (tag == "r0_12") {
                pt.put("run.r0", 12.0);
                target = 118342U;
        }
        if (tag == "r0_16") {
                pt.put("run.r0", 16.0);
                target = 119459;
        }
        return make_tuple(pt, target, sigma);
}

} // namespace Tests
