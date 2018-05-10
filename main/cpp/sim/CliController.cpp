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
 * Header for the command line controller.
 */

#include "sim/CliController.h"

#include "pop/Population.h"
#include "sim/SimRunner.h"
#include "sim/SimulatorBuilder.h"
#include "util/ConfigInfo.h"
#include "util/FileSys.h"
#include "util/LogUtils.h"
#include "util/TimeStamp.h"
#include "viewers/AdoptedViewer.h"
#include "viewers/CliViewer.h"
#include "viewers/InfectedViewer.h"
#include "viewers/PersonsViewer.h"
#include "viewers/SummaryViewer.h"

#include <boost/property_tree/xml_parser.hpp>
#include <gengeopop/GeoGrid.h>

using namespace std;
using namespace stride::util;
using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;

namespace stride {

CliController::CliController(const ptree& config_pt) : BaseController(config_pt) {}

void CliController::Control()
{
        // -----------------------------------------------------------------------------------------
        // Build population, instantiate SimRunner & register viewers & run.
        // -----------------------------------------------------------------------------------------
        m_runner->Run();
        m_stride_logger->info("CliController shutting down.");
        spdlog::drop_all();
}

} // namespace stride
