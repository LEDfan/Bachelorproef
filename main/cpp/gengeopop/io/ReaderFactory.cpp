#include "ReaderFactory.h"
#include "CitiesCSVReader.h"
#include "CommutesCSVReader.h"
#include "HouseholdCSVReader.h"
#include <boost/filesystem.hpp>
#include <util/FileSys.h>

namespace gengeopop {

std::shared_ptr<CitiesReader> ReaderFactory::createCitiesReader(const std::string& filename)
{
        return createCitiesReader(stride::util::FileSys::GetDataDir() / boost::filesystem::path(filename));
}

std::shared_ptr<CitiesReader> ReaderFactory::createCitiesReader(const boost::filesystem::path& path)
{

        if (path.extension().string() == ".csv") {
                std::ifstream file(path.string());
                return std::make_shared<CitiesCSVReader>(file);
        } else {
                throw std::runtime_error("No supported file extension: " + path.extension().string());
        }
}

std::shared_ptr<CommutesReader> ReaderFactory::createCommutesReader(const std::string& filename)
{
        return createCommutesReader(stride::util::FileSys::GetDataDir() / boost::filesystem::path(filename));
}

std::shared_ptr<CommutesReader> ReaderFactory::createCommutesReader(const boost::filesystem::path& path)
{
        if (path.extension().string() == ".csv") {
                std::ifstream file(path.string());
                return std::make_shared<CommutesCSVReader>(file);
        } else {
                throw std::runtime_error("No supported file extension: " + path.extension().string());
        }
}

HouseholdReader ReaderFactory::createHouseholdReader(const std::string& filename)
{
        return createHouseholdReader(stride::util::FileSys::GetDataDir() / boost::filesystem::path(filename));
}

HouseholdReader ReaderFactory::createHouseholdReader(const boost::filesystem::path& path)
{
        if (path.extension().string() == ".csv") {
                std::ifstream file(path.string());
                return HouseholdCSVReader(file);
        } else {
                throw std::runtime_error("No supported file extension: " + path.extension().string());
        }
}

} // namespace gengeopop
