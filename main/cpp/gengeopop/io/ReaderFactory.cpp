#include "ReaderFactory.h"
#include "CommutesCSVReader.h"
#include "CitiesCSVReader.h"
#include "HouseholdCSVReader.h"
#include <boost/filesystem.hpp>
#include <util/FileSys.h>

namespace gengeopop {

CitiesReader ReaderFactory::createCitiesReader(const std::string &filename) {
        return createCitiesReader(stride::util::FileSys::GetDataDir() /  boost::filesystem::path(filename));
}

CitiesReader ReaderFactory::createCitiesReader(const boost::filesystem::path &path) {

        if (path.extension().string() == ".csv") {
                std::ifstream file(path.string());
                return CitiesCSVReader(file);
        } else {
                throw std::runtime_error("No supported file extension: " + path.extension().string());
        }
}


CommutesReader ReaderFactory::createCommutesReader(const std::string &filename, const std::map<int, std::shared_ptr<Location>>& locations) {
        return createCommutesReader(stride::util::FileSys::GetDataDir() /  boost::filesystem::path(filename), locations);
}

CommutesReader ReaderFactory::createCommutesReader(const boost::filesystem::path &path, const std::map<int, std::shared_ptr<Location>>& locations) {
        if (path.extension().string() == ".csv") {
                std::ifstream file(path.string());
                return CommutesCSVReader(file, locations);
        } else {
                throw std::runtime_error("No supported file extension: " + path.extension().string());
        }
}

HouseholdReader ReaderFactory::createHouseholdReader(const std::string &filename) {
        return createHouseholdReader(stride::util::FileSys::GetDataDir() /  boost::filesystem::path(filename));
}

HouseholdReader ReaderFactory::createHouseholdReader(const boost::filesystem::path &path) {
        if (path.extension().string() == ".csv") {
                std::ifstream file(path.string());
                return HouseholdCSVReader(file);
        } else {
                throw std::runtime_error("No supported file extension: " + path.extension().string());
        }
}

}
