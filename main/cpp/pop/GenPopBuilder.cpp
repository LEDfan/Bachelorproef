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
 *  Copyright 2017, 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Initialize populations: implementation.
 */

#include "GenPopBuilder.h"

#include "Population.h"
#include "pop/Population.h"
#include "pop/SurveySeeder.h"
#include "util/FileSys.h"
#include "util/LogUtils.h"
#include "util/RNManager.h"
#include "util/StringUtils.h"

#include <boost/property_tree/ptree.hpp>
#include <gengeopop/GenGeoPopController.h>
#include <gengeopop/GeoGridConfig.h>
#include <spdlog/common.h>
#include <spdlog/fmt/ostr.h>

namespace stride {

using namespace std;
using namespace util;
using namespace boost::property_tree;
using namespace gengeopop;

shared_ptr<Population> GenPopBuilder::Build(std::shared_ptr<Population> pop)
{
        auto stride_logger = spdlog::get("stride_logger");

        //        if (!stride_logger) {
        //                std::cout << "Can't find stride logger" << std::endl;
        //        }
        //
        //        auto logger2 = LogUtils::CreateCliLogger("stride_logger", "");

        std::cout << "hoi" << std::endl;

        pop->m_belief_pt = m_config_pt.get_child("run.belief_policy");

        // --------------------------------------------------------------
        // Configure.
        // --------------------------------------------------------------
        GeoGridConfig geoGridConfig{};
        geoGridConfig.input.populationSize = m_config_pt.get<unsigned int>("run.geopop_gen.population_size");
        geoGridConfig.input.fraction_1826_years_WhichAreStudents =
            m_config_pt.get<double>("run.geopop_gen.fraction_1826_years_which_are_students");
        geoGridConfig.input.fraction_active_commutingPeople =
            m_config_pt.get<double>("run.geopop_gen.fraction_active_commuting_people");
        geoGridConfig.input.fraction_student_commutingPeople =
            m_config_pt.get<double>("run.geopop_gen.fraction_student_commuting_people");
        geoGridConfig.input.fraction_1865_years_active =
            m_config_pt.get<double>("run.geopop_gen.fraction_1865_years_active");

        GenGeoPopController genGeoPopController(stride_logger, geoGridConfig, m_rn_manager,
                                                m_config_pt.get<std::string>("run.geopop_gen.cities_file"),
                                                m_config_pt.get<std::string>("run.geopop_gen.commuting_file"),
                                                m_config_pt.get<std::string>("run.geopop_gen.household_file"),
                                                m_config_pt.get<std::string>("run.geopop_gen.submunicipalities_file"));

        genGeoPopController.UsePopulation(pop);

        // --------------------------------------------------------------
        // Read input files.
        // --------------------------------------------------------------
        genGeoPopController.ReadDataFiles();

        stride_logger->info("GeoGridConfig:\n\n{}", geoGridConfig);

        // --------------------------------------------------------------
        // Generate Geo
        // --------------------------------------------------------------
        stride_logger->info("Starting Gen-Geo");
        genGeoPopController.GenGeo();
        stride_logger->info("ContactCenters generated: {}", geoGridConfig.generated.contactCenters);
        stride_logger->info("ContactPools generated: {}", geoGridConfig.generated.contactPools);
        stride_logger->info("Finished Gen-Geo");

        // --------------------------------------------------------------
        // Generate Pop
        // --------------------------------------------------------------
        stride_logger->info("Starting Gen-Pop");
        genGeoPopController.GenPop();
        stride_logger->info("Finished Gen-Pop");

        pop->m_geoGrid = genGeoPopController.GetGeoGrid();

        return pop;
}

} // namespace stride
