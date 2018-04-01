#include "SecondaryCommunity.h"

namespace gengeopop {

SecondaryCommunity::SecondaryCommunity(unsigned int id) : Community(id) {}
std::string SecondaryCommunity::getType() const { return "Secondary Community"; }
} // namespace gengeopop
