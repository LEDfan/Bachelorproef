#pragma once

#include "IPartialGenerator.h"

namespace gengeopop {

class SchoolGenerator : public IPartialGenerator {
public:

    void apply(std::shared_ptr<GeoGrid> geogrid) override;

};
}
