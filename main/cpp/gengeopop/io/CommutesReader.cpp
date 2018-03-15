#include "CommutesReader.h"
#include <iostream>
#include <util/CSV.h>

namespace gengeopop {

CommutesReader::CommutesReader(std::istream& inputStream, const std::map<int, std::shared_ptr<Location>>& locations)
    : m_inputStream(inputStream) {
}
const std::map<std::shared_ptr<Location>,
               std::pair<std::shared_ptr<Location>, double>> &CommutesReader::getCommutes() const {
        return m_commutes;
}

} // namespace gengeopop
