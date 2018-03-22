#include "CitiesReader.h"

namespace gengeopop {

CitiesReader::CitiesReader(std::unique_ptr<std::istream> inputStream) : m_inputStream(std::move(inputStream)) {}

unsigned int CitiesReader::getTotalPopulation() const { return m_totalPopulation; }

} // namespace gengeopop
