#pragma once

#include "Exception.h"
#include "NestedIterator.h"
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
        using iterator               = NestedIterator<T, partitionIterator_type, SVIterator<T, N, T*, T&, false>>;
        using const_iterator = NestedIterator<T, partitionIterator_type, SVIterator<T, N, const T*, const T&, true>,
                                              const T*, const T&, true>;

        //// Construct
        explicit PartitionedSegmentedVector(std::size_t partitionsCount)
            : m_partitions(partitionsCount), m_prefixSums(), m_finalized(false)
        {
        }

        /// Copy constructor
        PartitionedSegmentedVector(const PartitionedSegmentedVector<T, N>&) = delete;

        /// Move constructor
        PartitionedSegmentedVector(const PartitionedSegmentedVector<T, N>&&) = delete;

        /// Copy assignment
        PartitionedSegmentedVector& operator=(const PartitionedSegmentedVector<T, N>& other) = delete;

        /// Move assignment
        PartitionedSegmentedVector& operator=(PartitionedSegmentedVector<T, N>&& other) noexcept = delete;

        ~PartitionedSegmentedVector() = default;

        iterator begin() { return iterator(m_partitions.begin(), m_partitions.begin(), m_partitions.end()); };

        iterator end() { return iterator(m_partitions.end(), m_partitions.begin(), m_partitions.end()); };

        const_iterator cbegin()
        {
                return const_iterator(m_partitions.begin(), m_partitions.begin(), m_partitions.end());
        };
        const_iterator cend() { return const_iterator(m_partitions.end(), m_partitions.begin(), m_partitions.end()); };

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
                auto lower = FindPartition(pos);

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
                auto lower = FindPartition(pos);

                // If no partition found
                if (lower == m_prefixSums.end()) {
                        throw std::out_of_range("index out of range.");
                }

                // The segmented vector will do a range check
                return std::get<0>(*lower)->at(pos - std::get<2>(*lower));
        }

        T& operator[](std::size_t pos)
        {
                auto lower = FindPartition(pos);
                return std::get<0>(*lower)[pos - std::get<2>(*lower)];
        }

        const T& operator[](std::size_t pos) const
        {
                auto lower = FindPartition(pos);
                return std::get<0>(*lower)[pos - std::get<2>(*lower)];
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

        segmentedVector_type& GetPartition(std::size_t partitionIndex) { return m_partitions[partitionIndex]; }

        const segmentedVector_type& GetPartition(std::size_t partitionIndex) const
        {
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
        /// Find in which partition pos lies
        typename std::vector<partItIndex>::iterator FindPartition(std::size_t pos)
        {
                if (!m_finalized) {
                        throw Exception("Must be finalized");
                }
                return std_::lower_bound(m_prefixSums.begin(), m_prefixSums.end(), pos,
                                         [](const partItIndex& current, std::size_t value) {
                                                 // - 1 because we are searching for index and prefixSum contains
                                                 // the size
                                                 return std::get<1>(current) - 1 < value;
                                         });
        }

        std::vector<segmentedVector_type> m_partitions; ///< Partitions
        std::vector<partItIndex>
             m_prefixSums; ///< Metadata with iterator, prefix sum and prefix sum without current size
        bool m_finalized;  ///< whether the partitions may grow or partitions may be added
};

} // namespace util
} // namespace stride
