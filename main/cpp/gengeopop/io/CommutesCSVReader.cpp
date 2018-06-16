#include "CommutesCSVReader.h"
#include <iostream>
#include <util/CSV.h>
#include <util/Exception.h>

namespace gengeopop {

CommutesCSVReader::CommutesCSVReader(std::unique_ptr<std::istream> inputStream)
    : CommutesReader(std::move(inputStream)), m_reader(*(m_inputStream.get()))
{
}

double CommutesCSVReader::MunicipalityTotal(std::shared_ptr<Location> loc) const
{
        double total = 0;
        for (const auto& sub : loc->GetSubMunicipalities()) {
                total += sub->GetRelativePopulationSize();
        }
        return total;
}

void CommutesCSVReader::AddCommute(std::shared_ptr<Location> from, std::shared_ptr<Location> to,
                                   double proportion) const
{
        from->AddOutgoingCommutingLocation(to, proportion);
        to->AddIncomingCommutingLocation(from, proportion);
}
void CommutesCSVReader::FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const
{
        // cols:
        // flanders_commuting format
        // kolom: stad van vertrek (headers = id)
        // rij: stad van aankomst (volgorde = volgorde van kolommen = id).

        // represents the location id for column x
        std::vector<unsigned int> headerMeaning;

        for (const std::string& label : m_reader.GetLabels()) {
                headerMeaning.push_back(static_cast<unsigned int>(stoi(label.substr(3))));
        }

        const size_t columnCount = m_reader.GetColumnCount();

        std::map<unsigned int, unsigned int> sizes; // indexed by header/row id

        /*
         * Since columns represent the "from city" and the proportion is calculated using the from city,
         * the total population of a city is calculated using the values found in the columns.
         */
        for (const stride::util::CSVRow& row : m_reader) {
                for (size_t columnIndex = 0; columnIndex < columnCount; columnIndex++) {
                        sizes[columnIndex] += row.GetValue<int>(columnIndex);
                }
        }

        size_t rowIndex = 0;
        for (const stride::util::CSVRow& row : m_reader) {
                for (size_t columnIndex = 0; columnIndex < columnCount; columnIndex++) {
                        auto abs = row.GetValue<double>(columnIndex);
                        if (abs != 0 && columnIndex != rowIndex) {
                                const auto& locFrom    = geoGrid->GetById(headerMeaning[columnIndex]);
                                const auto& locTo      = geoGrid->GetById(headerMeaning[rowIndex]);
                                const auto& total      = sizes[columnIndex];
                                double      proportion = abs / total;

                                if (proportion < 0 || proportion > 1) {
                                        throw stride::util::Exception(
                                            "Proportion of commutes from " + std::to_string(locFrom->GetID()) + " to " +
                                            std::to_string(locTo->GetID()) + " is invalid (0 <= proportion <= 1)");
                                }

                                const auto& subMunicipalities_from = locFrom->GetSubMunicipalities();
                                const auto& subMunicipalities_to   = locTo->GetSubMunicipalities();

                                for (auto& subMunicipalityFrom : subMunicipalities_from) {
                                        double total_population = MunicipalityTotal(locTo);
                                        if (total_population > 0) {
                                                for (auto& subMunicipalityTo : subMunicipalities_to) {
                                                        double prop = proportion *
                                                                      (subMunicipalityTo->GetRelativePopulationSize() /
                                                                       total_population);
                                                        AddCommute(subMunicipalityFrom, subMunicipalityTo, prop);
                                                }
                                        } else if (subMunicipalities_to.empty()) {
                                                AddCommute(subMunicipalityFrom, locTo, proportion);
                                        }
                                }
                                if (subMunicipalities_from.empty()) {
                                        double total_population = MunicipalityTotal(locTo);
                                        if (total_population > 0) {
                                                for (auto& subMunicipality : subMunicipalities_to) {
                                                        double prop =
                                                            proportion * (subMunicipality->GetRelativePopulationSize() /
                                                                          total_population);
                                                        AddCommute(locFrom, subMunicipality, prop);
                                                }
                                        } else if (subMunicipalities_to.empty()) {
                                                AddCommute(locFrom, locTo, proportion);
                                        }
                                }
                        }
                }
                rowIndex++;
        }
}

} // namespace gengeopop
