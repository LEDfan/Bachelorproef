#pragma once

#include "TravellerProfile.h"
#include <boost/property_tree/ptree.hpp>
#include <memory>
#include <pop/Population.h>

namespace stride {

/**
 * Builds a TravellerProfile from the values in a configPt (using csv files).
 */
class TravellerProfileBuilder
{
public:
        /// Constructor
        TravellerProfileBuilder(const boost::property_tree::ptree& configPt, util::RnMan& rnManager,
                                std::shared_ptr<Population> pop);

        /// Build and return the TravellerProfile
        std::shared_ptr<TravellerProfile> Build();

private:
        /// Call the provided updateMethod with the traveller information found in the configParam and the given regions
        void AddTravellerInformation(std::string configParam, std::unordered_map<std::string, std::size_t> regions,
                                     std::function<void(std::size_t, std::size_t, double)> updateMethod);

        const boost::property_tree::ptree& m_configPt;  ///< Configuration sued
        util::RnMan&                   m_rnManager; ///< RnManager (passed to created TravellerProfile)
        std::shared_ptr<Population>        m_pop;       ///< Population
};

} // namespace stride
