#include "CommutesReader.h"
#include <iostream>
#include <util/CSV.h>

namespace gengeopop {

CommutesReader::CommutesReader(std::istream& inputStream)
    : m_inputStream(inputStream) {
}
void CommutesReader::fillGeoGrid(std::shared_ptr<GeoGrid>) const {

}

} // namespace gengeopop
