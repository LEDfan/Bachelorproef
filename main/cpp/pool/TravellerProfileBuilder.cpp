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

#include "TravellerProfileBuilder.h"
#include "util/CSV.h"
#include <boost/property_tree/ptree.hpp>
#include <pop/Population.h>
#include <util/CSV.h>
#include <util/Exception.h>
#include <util/FileSys.h>
#include <util/RnMan.h>
#include <utility>

namespace stride {

TravellerProfileBuilder::TravellerProfileBuilder(const boost::property_tree::ptree& configPt, util::RnMan& rnManager,
                                                 std::shared_ptr<Population> pop)
    : m_configPt(configPt), m_rnManager(rnManager), m_pop(std::move(pop))
{
}

void TravellerProfileBuilder::AddTravellerInformation(
    std::string configParam, std::unordered_map<std::string, std::size_t> regions,
    std::function<void(std::size_t, std::size_t, double)> updateMethod)
{
        boost::optional<std::string> name = m_configPt.get_optional<std::string>(configParam);
        if (name) {
                const std::filesystem::path path = util::FileSys::GetDataDir() /= name.value();
                if (!std::filesystem::is_regular_file(path)) {
                        throw std::runtime_error(std::string(__func__) +
                                                 "Could not find file for travelling data: " + configParam);
                }

                stride::util::CSV reader(path);
                for (const stride::util::CSVRow& row : reader) {
                        const std::string& from     = row.GetValue<std::string>("from");
                        const std::string& to       = row.GetValue<std::string>("to");
                        auto               relative = row.GetValue<double>("relative");

                        updateMethod(regions.at(from), regions.at(to), relative);
                }
        }
}
std::shared_ptr<TravellerProfile> TravellerProfileBuilder::Build()
{
        std::unordered_map<std::string, std::size_t> regions = m_pop->GetRegionIdentifiers();

        double      amountOfTravel = m_configPt.get<double>("run.traveller_amount", 0);
        double      fractionWork   = m_configPt.get<double>("run.traveller_fraction_work_travel", 0.5);
        std::size_t maxDuration    = m_configPt.get<std::size_t>("run.traveller_max_duration", 21);

        if (maxDuration < 1) {
                throw util::Exception("Min duration for travel is one day");
        }

        auto travellerProfile =
            std::make_shared<TravellerProfile>(regions.size(), amountOfTravel, fractionWork, m_rnManager, maxDuration);

        AddTravellerInformation("run.traveller_data_recreation", regions,
                                std::bind(&TravellerProfile::AddTravelRecreation, travellerProfile,
                                          std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        AddTravellerInformation("run.traveller_data_work", regions,
                                std::bind(&TravellerProfile::AddTravelWork, travellerProfile, std::placeholders::_1,
                                          std::placeholders::_2, std::placeholders::_3));

        return travellerProfile;
}

} // namespace stride
