#include "ReaderFactory.h"
#include "CitiesCSVReader.h"
#include "CommutesCSVReader.h"
#include "HouseholdCSVReader.h"
#include <iostream>
#include <util/Exception.h>
#include <util/FileSys.h>

namespace gengeopop {

std::shared_ptr<CitiesReader> ReaderFactory::CreateCitiesReader(const std::string& filename)
{
        return CreateCitiesReader(stride::util::FileSys::GetDataDir() / std::filesystem::path(filename));
}

std::shared_ptr<CitiesReader> ReaderFactory::CreateCitiesReader(const std::filesystem::path& path)
{
        return std::make_shared<CitiesCSVReader>(OpenFile(path));
}

std::shared_ptr<CommutesReader> ReaderFactory::CreateCommutesReader(const std::string& filename)
{
        return CreateCommutesReader(stride::util::FileSys::GetDataDir() / std::filesystem::path(filename));
}

std::shared_ptr<CommutesReader> ReaderFactory::CreateCommutesReader(const std::filesystem::path& path)
{
        return std::make_shared<CommutesCSVReader>(OpenFile(path));
}

std::shared_ptr<HouseholdReader> ReaderFactory::CreateHouseholdReader(const std::string& filename)
{
        return CreateHouseholdReader(stride::util::FileSys::GetDataDir() / std::filesystem::path(filename));
}

std::shared_ptr<HouseholdReader> ReaderFactory::CreateHouseholdReader(const std::filesystem::path& path)
{
        return std::make_shared<HouseholdCSVReader>(OpenFile(path));
}

std::unique_ptr<std::istream> ReaderFactory::OpenFile(const std::filesystem::path& path) const
{
        if (!std::filesystem::exists(path)) {
                throw stride::util::Exception("File not found: " + path.string());
        }

        if (path.extension().string() == ".csv") {
                return std::make_unique<std::ifstream>(path.string());
        } else {
                throw stride::util::Exception("Unsupported file extension: " + path.extension().string());
        }
}

} // namespace gengeopop
