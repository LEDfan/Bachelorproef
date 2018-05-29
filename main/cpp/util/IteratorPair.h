#pragma once

namespace stride {
namespace util {

template <typename IteratorType>
class IteratorPair
{
public:
        IteratorPair(IteratorType begin, IteratorType end) : m_begin(begin), m_end(end) {}
        IteratorPair() : m_begin(), m_end() {}

        IteratorType begin() { return m_begin; }

        IteratorType end() { return m_end; }

private:
        IteratorType m_begin;
        IteratorType m_end;
};

} // namespace util
} // namespace stride
