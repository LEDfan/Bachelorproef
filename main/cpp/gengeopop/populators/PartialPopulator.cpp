#include "PartialPopulator.h"

namespace gengeopop {
PartialPopulator::PartialPopulator(stride::util::RNManager& rn_manager, std::shared_ptr<spdlog::logger> logger)
    : m_rnManager(rn_manager), m_logger(std::move(logger))
{
}
} // namespace gengeopop
