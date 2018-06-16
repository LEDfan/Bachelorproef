/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Implementation for the Immunizer class.
 */

#include "Immunizer.h"

#include "pop/Person.h"
#include "util/RNManager.h"

#include <trng/uniform01_dist.hpp>
#include <trng/uniform_int_dist.hpp>

namespace stride {

using namespace util;

Immunizer::Immunizer(stride::util::RNManager& rnManager) : m_rn_manager(rnManager) {}

//template<typename T>
//void Immunizer::Random(const T& pools, std::vector<double>& immunityDistribution,
//                       double immunityLinkProbability)
//{
//}

//template<typename T>
//void Immunizer::Cocoon(const T& /*pools*/,
//                       std::vector<double>& /*immunity_distribution*/, double /*immunity_link_probability*/)
//{
        /*
         * void Vaccinator::AdministerCocoon(const vector<ContactPool>& pools, double immunity_rate, double
adult_age_min, double adult_age_max, double child_age_min, double child_age_max)
{
// Sampler for double in [0.0, 1.0).
auto uniform01_generator = m_rn_manager.GetGenerator(trng::uniform01_dist<double>());
for (const auto& c : pools) {
        for (unsigned int i_p = 0; i_p < c.GetSize(); i_p++) {
                Person& p = *c.GetMember(i_p);
                if (p.GetHealth().IsSusceptible() && p.GetAge() >= adult_age_min &&
                    p.GetAge() <= adult_age_max) {

                        bool is_connected_to_target_age{false};
                        for (unsigned int i_p2 = 0; i_p2 < c.GetSize() && !is_connected_to_target_age; i_p2++) {
                                const Person& p2 = *c.GetMember(i_p2);
                                if (p2.GetAge() >= child_age_min && p2.GetAge() <= child_age_max) {
                                        is_connected_to_target_age = true;
                                }
                        }
                        if (is_connected_to_target_age && uniform01_generator() < immunity_rate) {
                                p.GetHealth().SetImmune();
                        }
                }
        }
}
}
         *
//         */
//}

} // namespace stride
