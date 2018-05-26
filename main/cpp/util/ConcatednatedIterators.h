#pragma once

#include "pchheader.h"
#include <vector>

namespace stride {
namespace util {

template <typename T, typename IteratorType, template <class...> class Storage = std::vector>
class ConcatenatedIterators
{
public:
        ConcatenatedIterators() : m_iterators() {}
        using iterator = NestedIterator<T, typename std::vector<IteratorPair<IteratorType>>::iterator, IteratorType>;

        iterator begin() { return iterator(m_iterators.begin(), m_iterators.begin(), m_iterators.end()); }

        iterator end() { return iterator(m_iterators.end(), m_iterators.begin(), m_iterators.end()); }

        void AddIterator(IteratorType begin, IteratorType end) { m_iterators.emplace_back(begin, end); }

private:
        Storage<IteratorPair<IteratorType>> m_iterators;
};

} // namespace util
} // namespace stride
