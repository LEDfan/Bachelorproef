#pragma once

#include "PSVIterator.h"
#include "SegmentedVector.h"

#include <vector>

namespace stride {
namespace util {

template <typename T, std::size_t N = 512>
class PartitionedSegmentedVector
{
public:
        //// Construct
        explicit PartitionedSegmentedVector(std::size_t partitionsCount) : m_partitions(partitionsCount) {}

        PSVIterator<T, N> begin() { return PSVIterator<T, N>(m_partitions.begin()); };

        PSVIterator<T, N> end() { return PSVIterator<T, N>(m_partitions.end()); };

        /// Copy constructor

        /// Move constructor

        /// Copy assignment

        /// Move assignment

        ~PartitionedSegmentedVector() = default;

        /// Adds element to end.
        T* push_back(std::size_t partition, const T& obj) { return m_partitions[partition].push_back(obj); }

        //        T& at(std::size_t pos) {
        //                for (const auto& partition : m_partitions) {
        //                        if (partition.size() >= pos) {
        //                               continue;
        //                        } else {
        //                                return partition[pos];
        //                        }
        //                }
        //
        //                throw std::out_of_range("index out of range.");
        //        }

        const T& at(std::size_t pos) const
        {
                for (const auto& partition : m_partitions) {
                        if (partition.size() >= pos) {
                                continue;
                        } else {
                                return partition[pos];
                        }
                }

                throw std::out_of_range("index out of range.");
        }

private:
        std::vector<SegmentedVector<T, N>> m_partitions;
};

} // namespace util
} // namespace stride