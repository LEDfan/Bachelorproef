#include "PartialPopulator.h"

namespace gengeopop {
PartialPopulator::PartialPopulator(stride::util::RNManager& rn_manager, std::shared_ptr<spdlog::logger> logger)
    : m_rnManager(rn_manager), m_logger(std::move(logger))
{
}

bool PartialPopulator::MakeChoice(double fraction)
{
        std::vector<double> weights;
        weights.push_back(1.0 - fraction); // -> 0, return is false -> not part of the fraction
        weights.push_back(fraction);       // -> 1, return is true -> part of the fraction

        auto dist = m_rnManager.GetGenerator(trng::discrete_dist(weights.begin(), weights.end()));
        return static_cast<bool>(dist());
}
} // namespace gengeopop
