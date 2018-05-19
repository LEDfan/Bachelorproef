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
template <typename T, typename OutItType, typename InnerItType, typename P = const T*, typename R = const T&,
          bool is_const_iterator = false>
class PSVIterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, P, R>
{
public:
        // ==================================================================
        // Member types (in addition to those introduced by the std::iterator
        // base class (i.e. value_type, difference_type, pointer, reference,
        // iterator_category).
        // ==================================================================
        using self_type = PSVIterator<T, OutItType, InnerItType, P, R, is_const_iterator>;
        //        using outerIterator_type = typename std::vector<SegmentedVector<T, N>>::iterator;
        PSVIterator() {}

        // ==================================================================
        // Construction / Copy / Move / Destruction
        // ==================================================================
        /// Default constructor
        explicit PSVIterator(OutItType outerBegin, OutItType outerEnd) : m_innerIsValid(false)
        {
                m_outerIterator = outerBegin;
                m_outerBegin    = outerBegin;
                m_outerEnd      = outerEnd;
                if (outerBegin != outerEnd) {
                        // only initialize inner when outer is not at the end
                        m_innerIterator = m_outerIterator->begin();
                        m_innerIsValid  = true;
                }
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
                ++m_innerIterator;
                if (m_innerIterator == m_outerIterator->end()) {
                        // When the innerIterator reaches the end, go to the next partition
                        ++m_outerIterator;
                        if (m_outerIterator != m_outerEnd) {
                                // The outerIterator is not at the end
                                // Set the innerIterator to the begin of the current partition
                                m_innerIterator = m_outerIterator->begin();
                        } else {
                                // Reached the end of the last partition
                                // The outerIterator is currently at the end of the partitions vector
                                // The innerIterator is invalid -> mark it as such so we don't dereference it
                                m_innerIsValid = false;
                        }
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

        /// Pre-decrement (returns position after decrement)
        self_type& operator--()
        {
                if (m_outerIterator == m_outerEnd) {
                        // When the iterator is at the end and will be decreased for the first time
                        --m_outerIterator;
                        m_innerIterator = --m_outerIterator->end();
                        m_innerIsValid  = true;
                } else {
                        if (m_innerIterator == m_outerIterator->begin()) {
                                // When the innerIterator reaches the begin, go to the previous partition
                                if (m_outerIterator == m_outerBegin) {
                                        // Reached before the begin of the first partition
                                        // Reset the outerIterator to m_outerBegin
                                        // Reset the innerIterator to the begin of the first partition
                                        m_outerIterator = m_outerBegin;
                                        m_innerIterator = m_outerIterator->begin();
                                } else {
                                        // The outerIterator is not at the end
                                        // Set the innerIterator to the last element of the previous partition
                                        --m_outerIterator;
                                        m_innerIterator = --m_outerIterator->end();
                                }
                        } else {
                                --m_innerIterator;
                        }
                }
                return *this;
        }

        /// Pre-increment (returns position after decrement)
        const self_type operator--(int)
        {
                self_type tmp(*this);
                          operator--();
                return tmp;
        }

        /// Iterator equality.
        bool operator==(const self_type& other) const
        {
                // Only check the inner if it's currently valid
                return m_outerIterator == other.m_outerIterator &&
                       (!m_innerIsValid || m_innerIterator == other.m_innerIterator);
        }

        /// Iterator inequality.
        bool operator!=(const self_type& other) const
        {
                // Only check the inner if it's currently valid
                return m_outerIterator != other.m_outerIterator ||
                       (m_innerIsValid && m_innerIterator != other.m_innerIterator);
        }

private:
        bool        m_innerIsValid;
        InnerItType m_innerIterator;

        OutItType m_outerIterator;
        OutItType m_outerEnd;
        OutItType m_outerBegin;
};

} // namespace util
} // namespace stride
