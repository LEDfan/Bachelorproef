#pragma once

#include "GeoGridConfig.h"
#include <gengeopop/io/CommutesReader.h>
#include <gengeopop/io/SubMunicipalitiesReader.h>
#include <util/RNManager.h>

namespace gengeopop {

class GenGeoPopController
{
public:
        GenGeoPopController(GeoGridConfig& geoGridConfig, stride::util::RNManager& rnManager,
                            std::string citiesFileName, std::string commutingFileName, std::string householdFileName,
                            std::string subMunicipalitiesFileName);

        void ReadDataFiles();

        void GengGeo();

        void GenPop();

        /// Get the generated GeoGrid
        std::shared_ptr<GeoGrid> GetGeoGrid();

private:
        GeoGridConfig& m_geoGridConfig; ///< The GeoGridConfig used to generate

        stride::util::RNManager& m_rnManager; ///< The RNManager used

        std::shared_ptr<GeoGrid> m_geoGrid; ///< The generated GeoGrid

        std::shared_ptr<CitiesReader>            m_citiesReader;            ///< The CitiesReader
        std::shared_ptr<CommutesReader>          m_commutesReader;          ///< The CommutesReader
        std::shared_ptr<HouseholdReader>         m_householdsReader;        ///< The HouseholdsReader
        std::shared_ptr<SubMunicipalitiesReader> m_subMunicipalitiesReader; ///< The SubMunicipalitiesReader

        std::string m_citiesFileName;
        std::string m_commutingFileName;
        std::string m_householdsFileName;
        std::string m_subMunicipalitiesFileName;
};

} // namespace gengeopop
