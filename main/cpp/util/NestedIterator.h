#pragma once
/**
 * @file
 * Interface/Implementation for NestedIterator.
 */

#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <limits>
#include <type_traits>
#include <vector>

namespace stride {
namespace util {

namespace {

/// Helper class to split out calls to outerIterator begin and end methods
/// so that cbegin or cend is called when the iterator is const.
template <typename OutItType, typename InnerItType, bool is_const_iterator>
class Helper
{
public:
        InnerItType outBegin(OutItType out);
        InnerItType outEnd(OutItType out);
};

template <typename OutItType, typename InnerItType>
class Helper<OutItType, InnerItType, false>
{
public:
        InnerItType outBegin(OutItType out) { return out->begin(); }

        InnerItType outEnd(OutItType out) { return out->end(); }
};

template <typename OutItType, typename InnerItType>
class Helper<OutItType, InnerItType, true>
{
public:
        InnerItType outBegin(OutItType out) { return out->cbegin(); }

        InnerItType outEnd(OutItType out) { return out->cend(); }
};
} // namespace

/**
 * Implementation of iterator for looping over the inner elements of a nested data structure.
 * For example, looping over the int's in std::vector<std::vector<int>>.
 * Provides both const and non-const iterators.
 *
 * Template parameters:
 * 	T         	  value type of inner iterator. This is the value which will be returned when the iterator is
 * dereferenced. OutItType         type of the outer iterator, this iterator will loop over the storage InnerItType
 * type of the inner iterator is_const_iterator to make it a const_iterator
 */
template <typename T, typename OutItType, typename InnerItType, typename P = const T*, typename R = const T&,
          bool is_const_iterator = false>
class NestedIterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, P, R>
{
public:
        // ==================================================================
        // Member types (in addition to those introduced by the std::iterator
        // base class (i.e. value_type, difference_type, pointer, reference,
        // iterator_category).
        // ==================================================================
        using self_type = NestedIterator<T, OutItType, InnerItType, P, R, is_const_iterator>;

        // ==================================================================
        // Construction / Copy / Move / Destruction
        // ==================================================================

        /// Default constructor
        NestedIterator()
            : m_innerIsValid(false), m_innerIterator(), m_outerIterator(), m_outerEnd(), m_outerBegin(), m_helper()
        {
        }

        /// Constructor
        NestedIterator(OutItType outerBegin, OutItType outerEnd)
            : m_innerIsValid(false), m_innerIterator(), m_outerIterator(outerBegin), m_outerEnd(outerEnd),
              m_outerBegin(outerBegin), m_helper()
        {
                if (outerBegin != outerEnd) {
                        // only initialize inner when outer is not at the end
                        m_innerIterator = m_helper.outBegin(m_outerIterator);
                        m_innerIsValid  = true;
                }
        }

        /// Copy constructor
        NestedIterator(const NestedIterator& other)
            : m_innerIsValid(other.m_innerIsValid), m_innerIterator(other.m_innerIterator),
              m_outerIterator(other.m_outerIterator), m_outerEnd(other.m_outerEnd), m_outerBegin(other.m_outerBegin),
              m_helper()
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
                ++m_innerIterator;
                if (m_innerIterator == m_helper.outEnd(m_outerIterator)) {
                        // When the innerIterator reaches the end, go to the next partition
                        ++m_outerIterator;
                        if (m_outerIterator != m_outerEnd) {
                                // The outerIterator is not at the end
                                // Set the innerIterator to the begin of the current partition
                                m_innerIterator = m_helper.outBegin(m_outerIterator);
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
                        m_innerIterator = --m_helper.outEnd(m_outerIterator);
                        m_innerIsValid  = true;
                } else {
                        if (m_innerIterator == m_helper.outBegin(m_outerIterator)) {
                                // When the innerIterator reaches the begin, go to the previous partition
                                if (m_outerIterator == m_outerBegin) {
                                        // Reached before the begin of the first partition
                                        // Reset the outerIterator to m_outerBegin
                                        // Reset the innerIterator to the begin of the first partition
                                        m_outerIterator = m_outerBegin;
                                        m_innerIterator = m_helper.outBegin(m_outerIterator);
                                } else {
                                        // The outerIterator is not at the end
                                        // Set the innerIterator to the last element of the previous partition
                                        --m_outerIterator;
                                        m_innerIterator = --m_helper.outEnd(m_outerIterator);
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
        bool        m_innerIsValid;  ///< Whether the m_innerIterator pointer is valid
        InnerItType m_innerIterator; ///< Iterator to the current inner value

        OutItType m_outerIterator; ///< Iterator to the current outer value
        OutItType m_outerEnd;      ///< End of the outer "storage"
        OutItType m_outerBegin;    ////< Begin of the outer "storage"

        Helper<OutItType, InnerItType, is_const_iterator> m_helper;
};

} // namespace util
} // namespace stride
