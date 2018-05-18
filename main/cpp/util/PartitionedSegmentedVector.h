#pragma once

#include "Exception.h"
#include "PSVIterator.h"
#include "SegmentedVector.h"
#include "lower_bound.h"

#include <numeric>
#include <tuple>
#include <vector>

namespace stride {
namespace util {

template <typename T, std::size_t N = 512>
class PartitionedSegmentedVector
{
public:
        using segmentedVector_type   = SegmentedVector<T, N>;
        using partitionIterator_type = typename std::vector<SegmentedVector<T, N>>::iterator;
        using partItIndex            = std::tuple<partitionIterator_type, std::size_t, std::size_t>;

        //// Construct
        explicit PartitionedSegmentedVector(std::size_t partitionsCount)
            : m_partitions(partitionsCount), m_prefixSums(), m_finalized(false)
        {
        }

        PSVIterator<T, N> begin() { return PSVIterator<T, N>(m_partitions.begin(), m_partitions.end()); };

        PSVIterator<T, N> end() { return PSVIterator<T, N>(m_partitions.end(), m_partitions.end()); };

        /// Copy constructor

        /// Move constructor

        /// Copy assignment

        /// Move assignment

        ~PartitionedSegmentedVector() = default;

        /// Adds element to end.
        T* push_back(std::size_t partition, const T& obj)
        {
                if (m_finalized) {
                        throw Exception("Must not be finalized");
                }
                return m_partitions[partition].push_back(obj);
        }

        /// Gets element at index with range check
        T& at(std::size_t pos)
        {
                if (!m_finalized) {
                        throw Exception("Must be finalized");
                }

                // Search which partition is needed in O(log(partitionsCount))
                auto lower = std_::lower_bound(m_prefixSums.begin(), m_prefixSums.end(), pos,
                                               [](const partItIndex& current, std::size_t value) {
                                                       // - 1 because we are searching for index and prefixSum contains
                                                       // the size
                                                       return std::get<1>(current) - 1 < value;
                                               });

                // If no partition found
                if (lower == m_prefixSums.end()) {
                        throw std::out_of_range("index out of range.");
                }

                // The segmented vector will do a range check
                return std::get<0>(*lower)->at(pos - std::get<2>(*lower));
        }

        /// Gets const element at index with range check
        const T& at(std::size_t pos) const
        {
                if (!m_finalized) {
                        throw Exception("Must be finalized");
                }

                // Search which partition is needed in O(log(partitionsCount))
                auto lower = std_::lower_bound(m_prefixSums.begin(), m_prefixSums.end(), pos,
                                               [](const partItIndex& current, std::size_t value) {
                                                       // - 1 because we are searching for index and prefixSum contains
                                                       // the size
                                                       return std::get<1>(current) - 1 < value;
                                               });

                // If no partition found
                if (lower == m_prefixSums.end()) {
                        throw std::out_of_range("index out of range.");
                }

                // The segmented vector will do a range check
                return std::get<0>(*lower)->at(pos - std::get<2>(*lower));
        }

        /// Calculates the size when not finalized else returns the size
        std::size_t size() const
        {
                if (m_finalized) {
                        return std::get<1>(m_prefixSums.back());
                } else {
                        return std::accumulate(m_partitions.begin(), m_partitions.end(), std::size_t(0),
                                               [](std::size_t currentSum, const segmentedVector_type& sv) {
                                                       return currentSum + sv.size();
                                               });
                }
        }

        segmentedVector_type& GetPartition(std::size_t partitionIndex) {
                return m_partitions[partitionIndex];
        }

        const segmentedVector_type& GetPartition(std::size_t partitionIndex) const {
                return m_partitions[partitionIndex];
        }

        /// Calculates the Indexes for the partitions and prevents partitions from being added or growing
        void Finalize()
        {
                std::size_t searchIndex = 0;
                for (auto it = m_partitions.begin(); it != m_partitions.end(); ++it) {
                        std::size_t sum = searchIndex + it->size();
                        m_prefixSums.emplace_back(it, sum, searchIndex);
                        it->Finalize();
                        searchIndex = sum;
                }
                m_finalized = true;
        }

        /// Whether the partitions may grow or partitions may be added
        bool IsFinalized() const { return m_finalized; }

private:
        std::vector<segmentedVector_type> m_partitions;
        std::vector<partItIndex>          m_prefixSums;
        bool                              m_finalized;
};

} // namespace util
} // namespace stride
