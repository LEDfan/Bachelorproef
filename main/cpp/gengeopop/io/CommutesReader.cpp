#include "CommutesReader.h"
#include <iostream>
#include <util/CSV.h>

namespace gengeopop {

CommutesReader::CommutesReader(std::istream& inputStream, const std::map<int, std::shared_ptr<Location>>& locations)
    : m_inputStream(inputStream)
{
        // cols:
        stride::util::CSV reader(m_inputStream);

        // flanders_commuting format
        // kolom: stad van vertrek (headers = id)
        // rij: stad van aankomst (volgorde = volgorde van kolommen = id).

        // represents the location id for column x
        std::vector<int> headerMeaning;

        for (const std::string& label : reader.getLabels()) {
                headerMeaning.push_back(stoi(label.substr(3)));
        }

        const size_t columnCount = reader.getColumnCount();

        size_t rowIndex = 0;
        for (const stride::util::CSVRow& row : reader) {
                for (size_t columnIndex = 0; columnIndex < columnCount; columnIndex++) {
                        const auto& locFrom = locations.at(headerMeaning[columnIndex]);
                        const auto& locTo   = locations.at(headerMeaning[rowIndex]);
                        double      abs     = stod(row.getValue(columnIndex));
                        if (abs != 0) {
                                double proprtion = abs / (double)locFrom->getPopulation();
                                m_commutes.insert({locFrom, {locTo, proprtion}});
                        }
                }
                rowIndex++;
        }
}

const std::map<std::shared_ptr<Location>, std::pair<std::shared_ptr<Location>, double>>& CommutesReader::getCommutes()
    const
{
        return m_commutes;
}

} // namespace gengeopop
