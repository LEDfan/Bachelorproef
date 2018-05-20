#pragma once

#include "Population.h"
#include <gengeopop/ContactPool.h>
#include <pool/ContactPoolType.h>
#include <string>
#include <util/NestedIterator.h>

class RegioSlicer
{
public:
        RegioSlicer(std::shared_ptr<stride::Population> pop);

        RegioSlicer& operator[](std::string regioName);
        RegioSlicer& operator[](stride::ContactPoolType::Id);

        typename stride::util::NestedIterator<
            stride::Person*, std::vector<stride::ContactPool, std::allocator<stride::ContactPool>>::iterator,
            std::vector<stride::Person*>::iterator>
        begin();
        typename stride::util::NestedIterator<
            stride::Person*, std::vector<stride::ContactPool, std::allocator<stride::ContactPool>>::iterator,
            std::vector<stride::Person*>::iterator>
        end();

        std::vector<stride::ContactPool, std::allocator<stride::ContactPool>>::iterator beginPools();
        std::vector<stride::ContactPool, std::allocator<stride::ContactPool>>::iterator endPools();

private:
        std::vector<stride::ContactPool>    m_pools;
        std::shared_ptr<stride::Population> m_pop;
};
