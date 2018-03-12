#pragma  once

#include <string>
#include <gengeopop/Location.h>
#include <map>

namespace gengeopop {
    class CommutesReader {
    public:
        std::map<Location, std::map<Location, unsigned int> >  getCommutes();
    private:
        std::map<Location, std::map<Location, unsigned int> > m_commutes;
    };
}

