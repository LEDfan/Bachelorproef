#pragma once


#include <string>
#include <pool/ContactPoolType.h>
#include <gengeopop/ContactPool.h>

class RegioSlicer {
public:
    RegioSlicer& operator[](std::string regioName);
    RegioSlicer& operator[](stride::ContactPoolType::Id);

    gengeopop::ContactPool::iterator begin();
    gengeopop::ContactPool::iterator end();

    gengeopop::ContactPool::iterator beginPools();
    gengeopop::ContactPool::iterator endPools();

private:

};