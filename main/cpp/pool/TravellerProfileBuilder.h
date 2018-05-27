#pragma once

#include "TravellerProfile.h"

namespace stride {

class TravellerProfileBuilder
{
public:
        TravellerProfileBuilder(const boost::property_tree::ptree& configPt, util::RNManager& rnManager,
                                std::shared_ptr<Population> pop);

        std::shared_ptr<TravellerProfile> Build();

private:
        const boost::property_tree::ptree& m_configPt;
        util::RNManager&                   m_rnManager;
        std::shared_ptr<Population>        m_pop;
};

} // namespace stride
