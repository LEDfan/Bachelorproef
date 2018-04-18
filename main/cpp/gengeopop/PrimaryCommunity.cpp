#include "PrimaryCommunity.h"

namespace gengeopop {

PrimaryCommunity::PrimaryCommunity(unsigned int id) : Community(id) {}
std::string PrimaryCommunity::GetType() const { return "Primary Community"; }

} // namespace gengeopop
