#pragma once

#include "HouseholdReader.h"
#include <pop/Population.h>

namespace gengeopop {
class HouseholdCSVReader : public HouseholdReader
{
public:
        explicit HouseholdCSVReader(std::unique_ptr<std::istream> inputStream);

private:
        ///< Persons used in this Household, segmented vector to be able to have working pointers to it
        stride::util::SegmentedVector<stride::Person> m_persons;

        ///< Contactpools used in this Household, segmented vector to be able to have working pointers to it
        stride::util::SegmentedVector<stride::ContactPool> m_contactPools;
};
} // namespace gengeopop
