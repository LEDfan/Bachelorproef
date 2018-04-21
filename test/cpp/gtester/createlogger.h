#pragma once

#include <spdlog/logger.h>
#include <util/LogUtils.h>

inline std::shared_ptr<spdlog::logger> createLogger()
{
        std::shared_ptr<spdlog::logger> logger = stride::util::LogUtils::CreateNullLogger("test_logger");
        return logger;
}
