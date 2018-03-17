#pragma once
#include "CitiesReader.h"
#include "CommutesReader.h"
#include "HouseholdReader.h"
#include <string>
#include <boost/filesystem/path.hpp>

namespace gengeopop {
class ReaderFactory
{
public:
        std::shared_ptr<CitiesReader>    createCitiesReader(const boost::filesystem::path& path);
        std::shared_ptr<CitiesReader>    createCitiesReader(const std::string& filename);

        /**
         * @param filename relative to data dir
         */
        std::shared_ptr<CommutesReader> createCommutesReader(const std::string& filename, const std::map<int, std::shared_ptr<Location>>& locations);
        /**
         * @param filename absolute path
         */
        std::shared_ptr<CommutesReader>  createCommutesReader(const boost::filesystem::path& path, const std::map<int, std::shared_ptr<Location>>& locations);

        HouseholdReader createHouseholdReader(const std::string& filename);
        HouseholdReader createHouseholdReader(const boost::filesystem::path& path);

};
} // namespace gengeopop
