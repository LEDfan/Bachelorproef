#include "CommutesCSVReader.h"
#include <iostream>
#include <util/CSV.h>

namespace gengeopop {

CommutesCSVReader::CommutesCSVReader(std::unique_ptr<std::istream> inputStream) : CommutesReader(std::move(inputStream)) {}

void CommutesCSVReader::FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const
{
        // cols:
        stride::util::CSV reader(*(m_inputStream.get()));

        // flanders_commuting format
        // kolom: stad van vertrek (headers = id)
        // rij: stad van aankomst (volgorde = volgorde van kolommen = id).

        // represents the location id for column x
        std::vector<unsigned int> headerMeaning;

        for (const std::string& label : reader.getLabels()) {
                headerMeaning.push_back(static_cast<unsigned int>(stoi(label.substr(3))));
        }

        const size_t columnCount = reader.getColumnCount();

        size_t rowIndex = 0;
        for (const stride::util::CSVRow& row : reader) {
                for (size_t columnIndex = 0; columnIndex < columnCount; columnIndex++) {
                        double abs = row.getValue<double>(columnIndex);
                        if (abs != 0 && columnIndex != rowIndex) {
                                const auto& locFrom    = geoGrid->GetById(headerMeaning[columnIndex]);
                                const auto& locTo      = geoGrid->GetById(headerMeaning[rowIndex]);
                                double      proportion = abs / (double) locFrom->getPopulation();
                                if (proportion < 0 || proportion > 1) {
                                        throw std::invalid_argument(
                                            "Proportion of commutes from " + std::to_string(locFrom->getID()) + " to " +
                                            std::to_string(locTo->getID()) + " is invalid (0 <= proportion <= 1)");
                                }
                                locFrom->addOutgoingCommutingLocation(locTo, proportion);
                                locTo->addIncomingCommutingLocation(locFrom, proportion);
                        }
                }
                rowIndex++;
        }
}

} // namespace gengeopop
