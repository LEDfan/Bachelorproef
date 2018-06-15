#pragma once

#include "HouseholdReader.h"
#include <pop/Population.h>

namespace gengeopop {

/**
 * Create a Reader that retrieves the different Household profiles from a given CSV file
 */
class HouseholdCSVReader : public HouseholdReader
{
public:
        /// Construct the HouseholdCSVReader with an istream containing the CSV data
        explicit HouseholdCSVReader(std::unique_ptr<std::istream> inputStream);

private:
        ///< Persons used in this Household, segmented vector to be able to have working pointers to it
        stride::util::SegmentedVector<stride::Person> m_persons;

        ///< Contactpools used in this Household, segmented vector to be able to have working pointers to it
        stride::util::SegmentedVector<stride::ContactPool> m_contactPools;
};
} // namespace gengeopop
