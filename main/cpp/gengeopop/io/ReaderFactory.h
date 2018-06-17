#pragma once
#include "CitiesReader.h"
#include "CommutesReader.h"
#include "HouseholdReader.h"
#include "SubMunicipalitiesReader.h"
#include <boost/filesystem/path.hpp>
#include <string>

namespace gengeopop {

/**
 * A Factory for creating the correct implementation of the different Readers which parse the information needed to
 * construct a GeoGrid, based on the file extension.
 * Currently only CSV Readers are implemented.
 */
class ReaderFactory
{
public:
        /// Create a CitiesReader based on an absolute Boost Filesystem Path pointing to the input file
        std::shared_ptr<CitiesReader> CreateCitiesReader(const boost::filesystem::path& path);

        /// Create a CitiesReader based on the filename of the input file, relative to the data directory
        std::shared_ptr<CitiesReader> CreateCitiesReader(const std::string& filename);

        /// Create a CommutesReader based on an absolute Boost Filesystem Path pointing to the input file
        std::shared_ptr<CommutesReader> CreateCommutesReader(const std::string& filename);

        /// Create a CommutesReader based on the filename of the input file, relative to the data directory
        std::shared_ptr<CommutesReader> CreateCommutesReader(const boost::filesystem::path& path);

        /// Create a HouseholdReader based on the filename of the input file, relative to the data directory
        std::shared_ptr<HouseholdReader> CreateHouseholdReader(const std::string& filename);

        /// Create a HouseholdReader based on an absolute Boost Filesystem Path pointing to the input file
        std::shared_ptr<HouseholdReader> CreateHouseholdReader(const boost::filesystem::path& path);

        /// Create a SubMunicipalitiesReader based on the filename of the input file, relative to the data directory
        std::shared_ptr<SubMunicipalitiesReader> CreateSubMunicipalitiesReader(const std::string& filename);

        /// Create a SubMunicipalitiesReader based on an absolute Boost Filesystem Path pointing to the input file
        std::shared_ptr<SubMunicipalitiesReader> CreateSubMunicipalitiesReader(const boost::filesystem::path& path);

private:
        /// Create an istream based on the provided Boost Filesystem Path
        std::unique_ptr<std::istream> OpenFile(const boost::filesystem::path& path) const;
};
} // namespace gengeopop
