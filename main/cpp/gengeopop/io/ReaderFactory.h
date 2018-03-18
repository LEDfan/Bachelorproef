#pragma once
#include "CitiesReader.h"
#include "CommutesReader.h"
#include "HouseholdReader.h"
#include <boost/filesystem/path.hpp>
#include <string>

namespace gengeopop {
class ReaderFactory
{
public:
        std::shared_ptr<CitiesReader> CreateCitiesReader(const boost::filesystem::path& path);
        std::shared_ptr<CitiesReader> CreateCitiesReader(const std::string& filename);

        /**
         * @param filename relative to data dir
         */
        std::shared_ptr<CommutesReader> CreateCommutesReader(const std::string& filename);
        /**
         * @param filename absolute path
         */
        std::shared_ptr<CommutesReader> CreateCommutesReader(const boost::filesystem::path& path);

        std::shared_ptr<HouseholdReader> CreateHouseholdReader(const std::string& filename);
        std::shared_ptr<HouseholdReader> CreateHouseholdReader(const boost::filesystem::path& path);
};
} // namespace gengeopop
