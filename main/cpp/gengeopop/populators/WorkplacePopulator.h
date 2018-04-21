#pragma once

#include "../GeoGridConfig.h"
#include "PartialPopulator.h"
#include <trng/uniform_int_dist.hpp>

namespace gengeopop {

namespace {
using discreteDist = std::function<trng::discrete_dist::result_type()>;
}

class WorkplacePopulator : public PartialPopulator
{
private:
        unsigned int m_assignedTo0          = 0;
        unsigned int m_assignedCommuting    = 0;
        unsigned int m_assignedNotCommuting = 0;

        std::shared_ptr<Location> m_currentLoc;

        std::shared_ptr<GeoGrid> m_geoGrid;
        GeoGridConfig            m_geoGridConfig;

        std::unordered_map<std::shared_ptr<Location>,
                           std::pair<std::vector<std::shared_ptr<ContactPool>>, discreteDist>>
            m_workplacesInCity;

        void calculateWorkplacesInCity();

        double m_fractionCommutingStudents;
        void   calculateFractionCommutingStudents();

        std::vector<std::shared_ptr<ContactPool>> m_nearByWorkplaces;
        discreteDist                              m_distNonCommuting;

        void calculateCommutingLocations();

        std::vector<std::shared_ptr<Location>> m_commutingLocations;
        discreteDist                           m_disCommuting;

        void calculateNearbyWorkspaces();

        void AssignActive(stride::Person* person);

public:
        WorkplacePopulator(stride::util::RNManager& rn_manager, std::shared_ptr<spdlog::logger> logger);

        void apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
