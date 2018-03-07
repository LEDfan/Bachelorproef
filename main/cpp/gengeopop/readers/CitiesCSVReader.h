#pragma once

#include "CitiesReader.h"

namespace gengeopop {
    class CitiesCSVReader : public CitiesReader {
    public:
        CitiesCSVReader(std::istream inputStream);

    };
}

