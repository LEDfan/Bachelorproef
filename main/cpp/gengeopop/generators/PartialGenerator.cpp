#include "PartialGenerator.h"
#include <spdlog/logger.h>

namespace gengeopop {
PartialGenerator::PartialGenerator(stride::util::RNManager& rn_manager, std::shared_ptr<spdlog::logger> logger)
    : m_rnManager(rn_manager), m_logger(std::move(logger))
{
}
} // namespace gengeopop
