#include "CommutesReader.h"
#include <iostream>
#include <util/CSV.h>

namespace gengeopop {

CommutesReader::CommutesReader(std::unique_ptr<std::istream> inputStream) : m_inputStream(std::move(inputStream)) {
}

} // namespace gengeopop
