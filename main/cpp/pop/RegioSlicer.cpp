#include "RegioSlicer.h"

RegioSlicer &RegioSlicer::operator[](std::string regioName) {
    // TODO make regionslicer refer to this region only
    return *this;
}

gengeopop::ContactPool::iterator RegioSlicer::begin() {
    return gengeopop::ContactPool::iterator();
}

gengeopop::ContactPool::iterator RegioSlicer::end() {
    return gengeopop::ContactPool::iterator();
}

gengeopop::ContactPool::iterator RegioSlicer::endPools() {
    return gengeopop::ContactPool::iterator();
}

gengeopop::ContactPool::iterator RegioSlicer::beginPools() {
    return gengeopop::ContactPool::iterator();
}
