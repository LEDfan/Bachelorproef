#include "SubMunicipalitiesReader.h"

namespace gengeopop {

SubMunicipalitiesReader::SubMunicipalitiesReader(std::unique_ptr<std::istream> inputStream)
    : m_inputStream(std::move(inputStream))
{
}

} // namespace gengeopop
