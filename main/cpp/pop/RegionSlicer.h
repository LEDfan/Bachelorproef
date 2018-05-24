#pragma once

#include <gengeopop/ContactPool.h>
#include <pool/ContactPoolType.h>
#include <string>
#include <util/NestedIterator.h>

namespace stride {

class Population;

class RegionSlicer
{
public:
        RegionSlicer(){};
        explicit RegionSlicer(stride::Population* pop);

        using person_iterator =
            typename stride::util::NestedIterator<stride::Person*, std::vector<stride::ContactPool>::iterator,
                                                  std::vector<stride::Person*>::iterator>;

        using contactpool_iterator = std::vector<stride::ContactPool>::iterator;

        RegionSlicer& operator[](const std::string& regioName);

        RegionSlicer& operator[](stride::ContactPoolType::Id);

        person_iterator begin();

        person_iterator end();

        contactpool_iterator beginPools();

        contactpool_iterator endPools();

private:
        void UpdatePools();

        std::vector<stride::ContactPool> m_pools;
        stride::Population*              m_pop;
};

} // namespace stride