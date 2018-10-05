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

#include "util/RunConfigManager.h"

using namespace std;
using namespace stride;
using namespace stride::util;
using boost::property_tree::ptree;

namespace Tests {

tuple<ptree, unsigned int, double> ScenarioData::Get(string tag)
{
        ptree pt = tag.substr(0, 2) != "r0" ? RunConfigManager::Create("TestsInfluenza")
                                            : RunConfigManager::Create("TestsMeasles");
        bool gengeopop = tag.size() > std::string("gengeopop").size() &&
                         tag.substr(tag.size() - std::string("gengeopop").size(), tag.size() - 1) == "gengeopop";

        bool multiregion = tag.size() > std::string("multiregion").size() &&
                           tag.substr(tag.size() - std::string("multiregion").size(), tag.size() - 1) == "multiregion";

        if (gengeopop)
                RunConfigManager::ConvertToGenGeoPop(pt);
        else if (multiregion)
                RunConfigManager::ConvertToMultiRegion(pt);

        std::map<std::string, unsigned int> targets_default = {
            {"influenza_a", 1085U},  {"influenza_b", 0U}, {"influenza_c", 5U}, {"measles_16", 599900U},
            {"measles_60", 600000U}, {"r0_0", 1200U},     {"r0_4", 39080U},    {"r0_8", 110884U},
            {"r0_12", 118342U},      {"r0_16", 119459U}};

        std::map<std::string, double> sigmas_default = {{"influenza_a", 116.60326645015648},
                                                        {"influenza_b", 0},
                                                        {"influenza_c", 0},
                                                        {"measles_16", 660.8227044636814},
                                                        {"measles_60", 0},
                                                        {"r0_0", targets_default["r0_0"] * 0.1},
                                                        {"r0_4", targets_default["r0_4"] * 0.1},
                                                        {"r0_8", targets_default["r0_8"] * 0.1},
                                                        {"r0_12", targets_default["r0_12"] * 0.1},
                                                        {"r0_16", targets_default["r0_16"] * 0.1}};

        std::map<std::string, unsigned int> targets_gengeopop = {
            {"influenza_a_gengeopop", 1276},  {"influenza_b_gengeopop", 0},     {"influenza_c_gengeopop", 5},
            {"measles_16_gengeopop", 599841}, {"measles_60_gengeopop", 600027}, {"r0_0_gengeopop", 1199},
            {"r0_12_gengeopop", 118125},      {"r0_16_gengeopop", 119442},      {"r0_4_gengeopop", 44391},
            {"r0_8_gengeopop", 111209}};
        std::map<std::string, double> sigmas_gengeopop = {{"influenza_a_gengeopop", 87.347638777473549},
                                                          {"influenza_b_gengeopop", 0},
                                                          {"influenza_c_gengeopop", 0},
                                                          {"measles_16_gengeopop", 510.60262435674969},
                                                          {"measles_60_gengeopop", 515.93547077129722},
                                                          {"r0_0_gengeopop", 1.6278820596099706},
                                                          {"r0_12_gengeopop", 166.31972222199025},
                                                          {"r0_16_gengeopop", 150.96290935193321},
                                                          {"r0_4_gengeopop", 919.14878556194594},
                                                          {"r0_8_gengeopop", 233.54134537593123}};

        std::map<std::string, unsigned int> targets_multiregion = {
            {"influenza_a_multiregion", 6550},   {"influenza_b_multiregion", 0},      {"influenza_c_multiregion", 27},
            {"measles_16_multiregion", 3065371}, {"measles_60_multiregion", 3065704}, {"r0_0_multiregion", 6132},
            {"r0_12_multiregion", 603882},       {"r0_16_multiregion", 610245},       {"r0_4_multiregion", 225930},
            {"r0_8_multiregion", 567296}};

        std::map<std::string, double> sigmas_multiregion = {{"influenza_a_multiregion", 102.22137741196799},
                                                            {"influenza_b_multiregion", 0.0},
                                                            {"influenza_c_multiregion", 0.0},
                                                            {"measles_16_multiregion", 1339.1634739642507},
                                                            {"measles_60_multiregion", 1108.1696666124733},
                                                            {"r0_0_multiregion", 2.5475478405713994},
                                                            {"r0_12_multiregion", 241.95073878787804},
                                                            {"r0_16_multiregion", 208.4690864372941},
                                                            {"r0_4_multiregion", 1570.369526576468},
                                                            {"r0_8_multiregion", 402.0301481232471}};
        unsigned int                  target;
        double                        sigma;
        if (gengeopop) {
                target = targets_gengeopop[tag];
                sigma  = sigmas_gengeopop[tag];
        } else if (multiregion) {
                target = targets_multiregion[tag];
                sigma  = sigmas_multiregion[tag];
        } else {
                target = targets_default[tag];
                sigma  = sigmas_default[tag];
        }
        if (gengeopop) {
                tag = tag.substr(0, tag.size() - std::string("_gengeopop").size());
        } else if (multiregion) {
                tag = tag.substr(0, tag.size() - std::string("_multiregion").size());
        }

        if (tag == "influenza_b") {
                pt.put("run.seeding_rate", 0.0);
        }
        if (tag == "influenza_c") {
                pt.put("run.seeding_rate", (1 - 0.9991) / 100);
                pt.put("run.immunity_rate", 0.9991);
        }
        if (tag == "measles_16") {
                pt.put("run.disease_config_file", "disease_measles.xml");
                pt.put("run.r0", 16U);
        }
        if (tag == "measles_60") {
                pt.put("run.disease_config_file", "disease_measles.xml");
                pt.put("run.r0", 60U);
        }

        if (tag == "r0_0") {
                pt.put("run.r0", 0.0);
        }
        if (tag == "r0_4") {
                pt.put("run.r0", 4.0);
        }
        if (tag == "r0_8") {
                pt.put("run.r0", 8.0);
        }
        if (tag == "r0_12") {
                pt.put("run.r0", 12.0);
        }
        if (tag == "r0_16") {
                pt.put("run.r0", 16.0);
        }
        return make_tuple(pt, target, sigma);
}

} // namespace Tests
