#include "TravellerProfileBuilder.h"
#include "util/CSV.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <pop/Population.h>
#include <util/CSV.h>
#include <util/FileSys.h>
#include <util/RNManager.h>
#include <utility>

namespace stride {

TravellerProfileBuilder::TravellerProfileBuilder(const boost::property_tree::ptree& configPt,
                                                 util::RNManager& rnManager, std::shared_ptr<Population> pop)
    : m_configPt(configPt), m_rnManager(rnManager), m_pop(std::move(pop))
{
}

std::shared_ptr<TravellerProfile> TravellerProfileBuilder::Build()
{
        std::unordered_map<std::string, std::size_t> regions = m_pop->GetRegionIdentifiers();

        auto travellerProfile = std::make_shared<TravellerProfile>(regions.size(), 0.0001, 0.5, m_rnManager, 21);

        {
                boost::optional<std::string> name =
                    m_configPt.get_optional<std::string>("run.traveller_data_recreation");
                if (name) {
                        const boost::filesystem::path path = util::FileSys::GetDataDir() /= name.value();
                        if (!boost::filesystem::is_regular_file(path)) {
                                throw std::runtime_error(std::string(__func__) + "Traveller data recreation not found");
                        }

                        stride::util::CSV reader(path);
                        for (const stride::util::CSVRow& row : reader) {
                                const std::string& from     = row.GetValue<std::string>("from");
                                const std::string& to       = row.GetValue<std::string>("to");
                                auto               relative = row.GetValue<double>("relative");

                                travellerProfile->AddTravelRecreation(regions.at(from), regions.at(to), relative);
                        }
                }
        }

        {
                boost::optional<std::string> name = m_configPt.get_optional<std::string>("run.traveller_data_work");
                if (name) {
                        const boost::filesystem::path path = util::FileSys::GetDataDir() /= name.value();

                        if (!boost::filesystem::is_regular_file(path)) {
                                throw std::runtime_error(std::string(__func__) + "Traveller data recreation not found");
                        }

                        stride::util::CSV reader(path);
                        for (const stride::util::CSVRow& row : reader) {
                                const std::string& from     = row.GetValue<std::string>("from");
                                const std::string& to       = row.GetValue<std::string>("to");
                                auto               relative = row.GetValue<double>("relative");

                                travellerProfile->AddTravelWork(regions.at(from), regions.at(to), relative);
                        }
                }
        }

        return travellerProfile;
}

} // namespace stride