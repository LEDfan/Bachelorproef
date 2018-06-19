#pragma once

#include "../GeoGridConfig.h"
#include "PartialPopulator.h"
#include <trng/uniform_int_dist.hpp>

namespace gengeopop {

namespace {
using discreteDist = std::function<trng::discrete_dist::result_type()>;
}

/// Populate Workplaces
class WorkplacePopulator : public PartialPopulator
{
public:
        /// Constructor
        WorkplacePopulator(stride::util::RNManager& rn_manager, std::shared_ptr<spdlog::logger> logger);

        /// Populates the workplaces in geogrid with persons
        void Apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) override;

private:
        unsigned int m_assignedTo0       = 0; ///< Amount of persons assigned to no workplace
        unsigned int m_assignedCommuting = 0; ///< Amount of persons assigned to a workplace outside the home location
        unsigned int m_assignedNotCommuting = 0; ///< Amount of persons assigned to a workplace in the home location

        std::shared_ptr<Location> m_currentLoc; ///< The location for which the workers currently are being assigned

        std::shared_ptr<GeoGrid> m_geoGrid;       ///< The GeoGrid which will be populated
        GeoGridConfig            m_geoGridConfig; ///< The GeoGridConfig used during populating

        std::unordered_map<std::shared_ptr<Location>, std::pair<std::vector<stride::ContactPool*>, discreteDist>>
            m_workplacesInCity; ///< For each location store the workplaces and a distribution to choose a random one

        /// Fills the m_workplacesInCity map
        void CalculateWorkplacesInCity();

        /// Fraction of the commuting people who are a student
        double m_fractionCommutingStudents;
        /// Calculates m_fractionCommutingStudents
        void CalculateFractionCommutingStudents();

        std::vector<stride::ContactPool*> m_nearByWorkplaces; ///< Workplaces which are nearby to the m_currentLoc
        discreteDist                      m_distNonCommuting; ///< distribution to choose from m_nearByWorkPlaces;

        /// Calculates the workplaces which are nearby to m_currentLoc
        void CalculateNearbyWorkspaces();

        std::vector<std::shared_ptr<Location>>
                     m_commutingLocations; ///< Workplaces which persons from m_currentLoc may commute to
        discreteDist m_disCommuting;       ///< distribution to choose from m_commutingLocations

        /// Calculates the workplaces to which persons from m_currentLoc may commute to
        void CalculateCommutingLocations();

        /// Assign a workplace to an active person
        void AssignActive(stride::Person* person);
};
} // namespace gengeopop
