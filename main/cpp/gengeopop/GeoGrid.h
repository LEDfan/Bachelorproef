#pragma once

#include <vector>

#include "Location.h"

namespace gengeopop {


    class GeoGrid {
    public:

            using iterator = std::vector<std::shared_ptr<Location>>::iterator;
            using const_iterator = std::vector<std::shared_ptr<Location>>::const_iterator;

            GeoGrid();

        void addLocation(std::shared_ptr<Location> location);

        std::vector<std::shared_ptr<Location>> findNearLocations(std::shared_ptr<Location> start, double range) const;

            /**
             * @param k
             * @return the K biggest Location of this GeoGrid
             */
            std::vector<std::shared_ptr<Location>> topK(size_t k) const;

            iterator begin();
            iterator end();

            const_iterator cbegin() const;
            const_iterator cend() const;

            size_t size() const;

        std::shared_ptr<Location> operator[](size_t index);

        std::shared_ptr<Location> get(size_t index);

    private:
        std::vector<std::shared_ptr<Location> > m_locations;

    };
}

