#include "CommutesCSVReader.h"
#include <iostream>
#include <util/CSV.h>

namespace gengeopop {

CommutesCSVReader::CommutesCSVReader(std::unique_ptr<std::istream> inputStream)
    : CommutesReader(std::move(inputStream)), m_reader(*(m_inputStream.get()))
{
}

void CommutesCSVReader::FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const
{
        // cols:
        // flanders_commuting format
        // kolom: stad van vertrek (headers = id)
        // rij: stad van aankomst (volgorde = volgorde van kolommen = id).

        // represents the location id for column x
        std::vector<unsigned int> headerMeaning;

        for (const std::string& label : m_reader.getLabels()) {
                headerMeaning.push_back(static_cast<unsigned int>(stoi(label.substr(3))));
        }

        const size_t columnCount = m_reader.getColumnCount();

        size_t rowIndex = 0;
        for (const stride::util::CSVRow& row : m_reader) {
                int total = 0;
                for (size_t columnIndex = 0; columnIndex < columnCount; columnIndex++) {
                        total += row.getValue<int>(columnIndex);
                }

                for (size_t columnIndex = 0; columnIndex < columnCount; columnIndex++) {
                        auto abs = row.getValue<double>(columnIndex);
                        if (abs != 0 && columnIndex != rowIndex) {
                                const auto& locFrom    = geoGrid->GetById(headerMeaning[columnIndex]);
                                const auto& locTo      = geoGrid->GetById(headerMeaning[rowIndex]);
                                double      proportion = abs / total;
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
