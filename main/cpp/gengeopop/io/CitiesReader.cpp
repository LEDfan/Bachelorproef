#include "CitiesReader.h"

namespace gengeopop {

CitiesReader::CitiesReader(std::unique_ptr<std::istream> inputStream) : m_inputStream(std::move(inputStream)) {}

} // namespace gengeopop
