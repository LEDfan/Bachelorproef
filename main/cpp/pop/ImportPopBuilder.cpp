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

#include "ImportPopBuilder.h"

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
#include <gengeopop/io/GeoGridReaderFactory.h>
#include <spdlog/common.h>
#include <spdlog/fmt/ostr.h>

namespace stride {

using namespace std;
using namespace util;
using namespace boost::property_tree;
using namespace gengeopop;

shared_ptr<Population> ImportPopBuilder::Build(std::shared_ptr<Population> pop)
{
        auto stride_logger = spdlog::get("stride_logger");

        std::string importFile = m_config_pt.get<std::string>("run.geopop_import_file");

        GeoGridReaderFactory                  geoGridReaderFactory;
        const std::shared_ptr<GeoGridReader>& reader = geoGridReaderFactory.CreateReader(importFile);

        stride_logger->debug("Importing population from " + importFile);

        pop->m_belief_pt = m_config_pt.get_child("run.belief_policy");
        reader->UsePopulation(pop);

        pop->m_geoGrid = reader->Read();
        pop->m_geoGrid->Finalize();

        return pop;
}

} // namespace stride
