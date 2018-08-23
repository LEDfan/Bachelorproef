#include "TravellerProfileBuilder.h"
#include "util/CSV.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
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
                const boost::filesystem::path path = util::FileSys::GetDataDir() /= name.value();
                if (!boost::filesystem::is_regular_file(path)) {
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

        double      amountOfTravel = m_configPt.get<double>("run.traveller_amount", 0.00001);
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
