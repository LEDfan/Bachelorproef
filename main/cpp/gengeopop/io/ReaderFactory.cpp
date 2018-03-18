#include "ReaderFactory.h"
#include "CitiesCSVReader.h"
#include "CommutesCSVReader.h"
#include "HouseholdCSVReader.h"
#include <boost/filesystem.hpp>
#include <util/FileSys.h>

namespace gengeopop {

std::shared_ptr<CitiesReader> ReaderFactory::CreateCitiesReader(const std::string& filename)
{
        return CreateCitiesReader(stride::util::FileSys::GetDataDir() / boost::filesystem::path(filename));
}

std::shared_ptr<CitiesReader> ReaderFactory::CreateCitiesReader(const boost::filesystem::path& path)
{

        if (path.extension().string() == ".csv") {
                std::ifstream file(path.string());
                return std::make_shared<CitiesCSVReader>(file);
        } else {
                throw std::runtime_error("Unsupported file extension: " + path.extension().string());
        }
}

std::shared_ptr<CommutesReader> ReaderFactory::CreateCommutesReader(const std::string& filename)
{
        return CreateCommutesReader(stride::util::FileSys::GetDataDir() / boost::filesystem::path(filename));
}

std::shared_ptr<CommutesReader> ReaderFactory::CreateCommutesReader(const boost::filesystem::path& path)
{
        if (path.extension().string() == ".csv") {
                std::ifstream file(path.string());
                return std::make_shared<CommutesCSVReader>(file);
        } else {
                throw std::runtime_error("Unsupported file extension: " + path.extension().string());
        }
}

std::shared_ptr<HouseholdReader> ReaderFactory::CreateHouseholdReader(const std::string& filename)
{
        return CreateHouseholdReader(stride::util::FileSys::GetDataDir() / boost::filesystem::path(filename));
}

std::shared_ptr<HouseholdReader> ReaderFactory::CreateHouseholdReader(const boost::filesystem::path& path)
{
        if (path.extension().string() == ".csv") {
                std::ifstream file(path.string());
                return std::make_shared<HouseholdCSVReader>(file);
        } else {
                throw std::runtime_error("Unsupported file extension: " + path.extension().string());
        }
}

} // namespace gengeopop
