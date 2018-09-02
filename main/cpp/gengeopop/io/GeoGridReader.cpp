#include <gengeopop/io/GeoGridReader.h>

namespace gengeopop {

GeoGridReader::GeoGridReader(std::unique_ptr<std::istream> inputStream)
    : m_people(), m_commutes(), m_regionId(0), m_regionName("Default"), m_inputStream(std::move(inputStream))
{
}
void GeoGridReader::AddCommutes(std::shared_ptr<GeoGrid> geoGrid)
{
        for (const auto& commute_tuple : m_commutes) {
                auto a      = geoGrid->GetById(std::get<0>(commute_tuple));
                auto b      = geoGrid->GetById(std::get<1>(commute_tuple));
                auto amount = std::get<2>(commute_tuple);
                a->AddOutgoingCommutingLocation(b, amount);
                b->AddIncomingCommutingLocation(a, amount);
        }
}

void GeoGridReader::UsePopulation(const std::shared_ptr<stride::Population>& pop, std::size_t regionId,
                                  std::string regionName)
{
        m_population = pop;
        m_regionId   = regionId;
        m_regionName = std::move(regionName);
}
} // namespace gengeopop
