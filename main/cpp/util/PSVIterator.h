#pragma once
/**
 * @file
 * Interface/Implementation for PSVIterator.
 */

#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <limits>
#include <type_traits>
#include <vector>

#include "SVIterator.h"
//#include "SegmentedVector.h"

namespace stride {
namespace util {

template <typename T, size_t N>
class PartitionedSegmentedVector;

template <typename T, size_t N>
class SegmentedVector;

/**
 * Implementation of iterator for SegmentedVector. It will provide
 * both const and non-const iterators.
 *
 * Possible states for the iterator are:
 * (a) Default constructed: m_c == nullptr && m_p == m_end. This is
 * the singular state in which the iterator can be assigned, but not
 * incremented or compared.
 * (b) Past-the-end: m_c != nullptr && m_p == m_end. The iterator
 * cannot be dereferenced.
 * (c) Dereferencable: m_c != nullptr && m_p < mc->size(). Notice that
 * m_p is of type size_t and hence always non-negative. Thus the above
 * reuires !m_c->empty().
 *
 * Template parameters:
 * 	T	value type of iterator and of its container.
 * 	N       block size of its container
 * 	P	pointer-to-T type (can be const qualified).
 * 	R	reference-to-T type (can be const qualified).
 * 	is_const_iterator	to make it a const_iterator
 */
template <typename T, std::size_t N, typename P = const T*, typename R = const T&, bool is_const_iterator = true>
class PSVIterator : public std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t, P, R>
{
public:
        // ==================================================================
        // Member types (in addition to those introduced by the std::iterator
        // base class (i.e. value_type, difference_type, pointer, reference,
        // iterator_category).
        // ==================================================================
        using self_type = PSVIterator<T, N, P, R, is_const_iterator>;

        // ==================================================================
        // Construction / Copy / Move / Destruction
        // ==================================================================
        /// Default constructor
        explicit PSVIterator(typename std::vector<SegmentedVector<T, N>>::iterator outerIterator)
            : m_innerIterator(m_outerIterator->begin()), m_outerIterator(outerIterator)
        {
        }

        // ==================================================================
        // Bidirectional iterator methods
        // ==================================================================

        /// Element access.
        R operator*() const { return *m_innerIterator; }

        /// Member of element access.
        P operator->() const { return m_innerIterator->operator->(); }

        /// Pre-increment (returns position after increment)
        self_type& operator++()
        {
                m_innerIterator++;
                if (m_innerIterator == m_outerIterator->end()) {
                        m_outerIterator++;
                        m_innerIterator = m_outerIterator->begin();
                }
                return *this;
        }

        /// Post-increment (returns position prior to increment)
        const self_type operator++(int)
        {
                self_type tmp(*this);
                          operator++();
                return tmp;
        }

        //        /// Pre-decrement (returns position after decrement)
        //        self_type& operator--()
        //        {
        //                return *this;
        //        }

        //        /// Pre-increment (returns position after decrement)
        //        const self_type operator--(int)
        //        {
        //                self_type tmp(*this);
        //                          operator--();
        //                return tmp;
        //        }

        /// Iterator equality.
        bool operator==(const self_type& other) const
        {
                return m_outerIterator == other.m_outerIterator && m_innerIterator == other.m_innerIterator;
        }

        /// Iterator inequality.
        bool operator!=(const self_type& other) const
        {
                return m_outerIterator != other.m_outerIterator || m_innerIterator != other.m_innerIterator;
        }

        // ==================================================================
        // Random-Access iterator methods
        // ==================================================================

        //        /// Direct access to n-th element
        //        R operator[](std::size_t n) const
        //        {
        //        }

        //        /// Set iterator to n-th next element.
        //        self_type& operator+=(std::ptrdiff_t n)
        //        {
        //                return *this;
        //        }

        //        /// Set iterator to n-th previous element.
        //        self_type& operator-=(std::ptrdiff_t n)
        //        {
        //                return *this;
        //        }

        //        /// Return iterator pointing to n-th next element.
        //        self_type operator+(std::ptrdiff_t n) { }

        //        /// Return iterator pointing to n-th previous element.
        //        //  self_type operator-(std::ptrdiff_t);

        //        /// Return distance between iterators.
        //        long int operator-(const self_type& other) const { return m_p - other.m_p; }

        //        /// Returns whether iterator is before other.
        //        bool operator<(const self_type& other) const { return m_p < other.m_p; }

        //        /// Returns whether iterator is not after other.
        //        bool operator<=(const self_type& other) const { return m_p <= other.m_p; }

        //        /// Returns whether iterator is after other.
        //        bool operator>(const self_type& other) const { return m_p > other.m_p; }

        //        /// Returns whether iterator is not after other.
        //        bool operator>=(const self_type& other) const { return m_p >= other.m_p; }

private:
        SVIterator<T, N, T*, T&, false> m_innerIterator;

        typename std::vector<SegmentedVector<T, N>>::iterator m_outerIterator;
};

} // namespace util
} // namespace stride
