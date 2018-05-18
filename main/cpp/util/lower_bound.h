#pragma once

#include <iterator>

namespace stride {
namespace util {
namespace std_ {

/**
 * Returns an iterator pointing to the first element in the range [first, last) that is not less than
 * (i.e. greater or equal to) value, or last if no such element is found.
 *
 * @see http://en.cppreference.com/w/cpp/algorithm/lower_bound
 * Implementation copied from ^, available in c++20
 */
template <class ForwardIt, class T, class Compare>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
{
        ForwardIt                                                 it;
        typename std::iterator_traits<ForwardIt>::difference_type count, step;
        count = std::distance(first, last);

        while (count > 0) {
                it   = first;
                step = count / 2;
                std::advance(it, step);
                if (comp(*it, value)) {
                        first = ++it;
                        count -= step + 1;
                } else
                        count = step;
        }
        return first;
}

} // namespace std_
} // namespace util
} // namespace stride
