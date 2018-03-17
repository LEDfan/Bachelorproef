#include "CommutesCSVReader.h"
#include <iostream>
#include <util/CSV.h>

namespace gengeopop {

CommutesCSVReader::CommutesCSVReader(std::istream& inputStream) : CommutesReader(inputStream) {

}
void CommutesCSVReader::fillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const {
        // cols:
        stride::util::CSV reader(m_inputStream);

        // flanders_commuting format
        // kolom: stad van vertrek (headers = id)
        // rij: stad van aankomst (volgorde = volgorde van kolommen = id).

        // represents the location id for column x
        std::vector<unsigned int> headerMeaning;

        for (const std::string& label : reader.getLabels()) {
                headerMeaning.push_back(static_cast<unsigned  int>(stoi(label.substr(3))));
        }

        const size_t columnCount = reader.getColumnCount();

        size_t rowIndex = 0;
        for (const stride::util::CSVRow& row : reader) {
                for (size_t columnIndex = 0; columnIndex < columnCount; columnIndex++) {
                        double      abs     = stod(row.getValue(columnIndex));
                        if (abs != 0 && columnIndex != rowIndex) {
                                const auto& locFrom = geoGrid->getById(headerMeaning[columnIndex]);
                                const auto& locTo   = geoGrid->getById(headerMeaning[rowIndex]);
                                double proprtion = abs / (double) locFrom->getPopulation();
                                // TODO check if 0 < proportion <= 1
                                locFrom->addOutgoingCommutingLocation(locTo, proprtion);
                                locTo->addIncomingCommutingLocation(locFrom, proprtion);
                        }
                }
                rowIndex++;
        }
}

} // namespace gengeopop
