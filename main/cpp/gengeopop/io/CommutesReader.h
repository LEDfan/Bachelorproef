#pragma once

#include <gengeopop/Location.h>
#include <map>
#include <string>

namespace gengeopop {
class CommutesReader
{
public:
        CommutesReader(std::istream& inputStream, const std::map<int, std::shared_ptr<Location>>& locations);

        const std::map<std::shared_ptr<Location>, std::pair<std::shared_ptr<Location>, double>>& getCommutes() const;

protected:
        std::map<std::shared_ptr<Location>, std::pair<std::shared_ptr<Location>, double>> m_commutes;
        std::istream&                                                                     m_inputStream;
};
} // namespace gengeopop
