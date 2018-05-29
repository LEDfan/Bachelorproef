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

shared_ptr<Population> GenPopBuilder::Build(std::shared_ptr<Population> pop, std::size_t regionId)
{
        auto stride_logger = spdlog::get("stride_logger");

        // --------------------------------------------------------------
        // Configure.
        // --------------------------------------------------------------
        GeoGridConfig geoGridConfig{};
        geoGridConfig.input.populationSize = m_region_pt.get<unsigned int>("geopop_gen.population_size");
        geoGridConfig.input.fraction_1826_years_WhichAreStudents =
            m_region_pt.get<double>("geopop_gen.fraction_1826_years_which_are_students");
        geoGridConfig.input.fraction_active_commutingPeople =
            m_region_pt.get<double>("geopop_gen.fraction_active_commuting_people");
        geoGridConfig.input.fraction_student_commutingPeople =
            m_region_pt.get<double>("geopop_gen.fraction_student_commuting_people");
        geoGridConfig.input.fraction_1865_years_active =
            m_region_pt.get<double>("geopop_gen.fraction_1865_years_active");

        std::string commutesFile;
        // Check if given
        auto geopop_gen = m_region_pt.get_child("geopop_gen");
        if (geopop_gen.count("commuting_file")) {
                commutesFile = m_region_pt.get<std::string>("geopop_gen.commuting_file");
        }

        std::string submunicipalitiesFile;
        // Check if given
        if (geopop_gen.count("submunicipalities_file") > 0) {
                submunicipalitiesFile = m_region_pt.get<std::string>("geopop_gen.submunicipalities_file");
        }

        GenGeoPopController genGeoPopController(
            stride_logger, geoGridConfig, m_rn_manager, m_region_pt.get<std::string>("geopop_gen.cities_file"),
            commutesFile, m_region_pt.get<std::string>("geopop_gen.household_file"), submunicipalitiesFile);

        genGeoPopController.UsePopulation(pop, regionId);

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

        pop->m_geoGrids.push_back(genGeoPopController.GetGeoGrid());

        return pop;
}

} // namespace stride
