#include <gengeopop/io/GeoGridReader.h>

namespace gengeopop {

GeoGridReader::GeoGridReader(std::unique_ptr<std::istream> inputStream)
    : m_people(), m_commutes(), m_subMunicipalities(), m_regionId(0), m_regionName("Default"),
      m_inputStream(std::move(inputStream))
{
}

void GeoGridReader::AddSubMunicipalities(std::shared_ptr<GeoGrid> geoGrid)
{
        for (const auto& subMunTuple : m_subMunicipalities) {
                geoGrid->GetById(subMunTuple.first)->AddSubMunicipality(geoGrid->GetById(subMunTuple.second));
        }
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

} // namespace gengeopop
