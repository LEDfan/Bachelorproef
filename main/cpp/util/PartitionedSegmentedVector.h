#pragma once

#include "Exception.h"
#include "util/pchheader.h"

#include <numeric>
#include <tuple>
#include <vector>

namespace stride {
namespace util {

template <typename T, std::size_t N = 512>
class PartitionedSegmentedVector
{
public:
        using segmentedVector_type   = SegmentedVector<T, N, true>;
        using partitionIterator_type = typename std::vector<SegmentedVector<T, N, true>>::iterator;
        using partItIndex            = std::tuple<partitionIterator_type, std::size_t, std::size_t>;
        using iterator               = NestedIterator<T, partitionIterator_type, SVIterator<T, N, true, T*, T&, false>>;
        using const_partitionIterator_type = typename std::vector<SegmentedVector<T, N, true>>::const_iterator;
        using const_iterator =
            NestedIterator<T, const_partitionIterator_type, SVIterator<T, N, true, const T*, const T&, true>, const T*,
                           const T&, true>;

        //// Construct
        explicit PartitionedSegmentedVector(std::size_t partitionsCount)
            : m_partitions(partitionsCount), m_prefixSums(), m_finalized(false)
        {
        }

        void CreatePartitions(std::size_t count) { m_partitions = std::vector<segmentedVector_type>(count); }

        //// Construct
        PartitionedSegmentedVector() : m_partitions(1), m_prefixSums(), m_finalized(false) {}

        /// Copy constructor
        PartitionedSegmentedVector(const PartitionedSegmentedVector<T, N>& other)
            : m_partitions(std::move(other.m_partitions)), m_prefixSums(std::move(other.m_prefixSums)),
              m_finalized(other.m_finalized)
        {
                m_partitions = other.m_partitions;
                m_prefixSums = other.m_prefixSums;
                m_finalized  = other.m_finalized;
        };

        /// Move constructor
        PartitionedSegmentedVector(const PartitionedSegmentedVector<T, N>&& other)
            : m_partitions(std::move(other.m_partitions)), m_prefixSums(std::move(other.m_prefixSums)),
              m_finalized(other.m_finalized){};

        /// Copy assignment
        PartitionedSegmentedVector& operator=(const PartitionedSegmentedVector<T, N>& other)
        {
                if (this != &other) {
                        m_partitions = other.m_partitions;
                        m_prefixSums = other.m_prefixSums;
                        m_finalized  = other.m_finalized;
                }
                return *this;
        };

        /// Move assignment
        PartitionedSegmentedVector& operator=(PartitionedSegmentedVector<T, N>&& other) noexcept
        {
                if (this != &other) {
                        m_partitions = other.m_partitions;
                        m_prefixSums = other.m_prefixSums;
                        m_finalized  = other.m_finalized;
                }
                return *this;
        };

        virtual ~PartitionedSegmentedVector() = default;

        iterator begin() { return iterator(m_partitions.begin(), m_partitions.begin(), m_partitions.end()); };

        iterator end() { return iterator(m_partitions.end(), m_partitions.begin(), m_partitions.end()); };

        const_iterator begin() const { return cbegin(); };

        const_iterator end() const { return cend(); }

        const_iterator cbegin() const
        {
                return const_iterator(m_partitions.begin(), m_partitions.begin(), m_partitions.end());
        };
        const_iterator cend() const
        {
                return const_iterator(m_partitions.end(), m_partitions.begin(), m_partitions.end());
        };

        /// Adds element to end.
        T* push_back(const T& obj, std::size_t partition = 0)
        {
                if (m_finalized) {
                        throw Exception("Must not be finalized");
                }
                return m_partitions[partition].push_back(obj);
        }

        /// Adds element to end.
        template <class... Args>
        T* emplace_back(Args&&... args)
        {
                if (m_finalized) {
                        throw Exception("Must not be finalized");
                }
                return m_partitions[0].emplace_back(std::forward<Args>(args)...);
        }

        /// Adds element to end.
        template <class... Args>
        T* emplace_back(std::size_t partition, Args&&... args)
        {
                if (m_finalized) {
                        throw Exception("Must not be finalized");
                }
                return m_partitions[partition].emplace_back(std::forward<Args>(args)...);
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

        /// Gets const element at index without range check
        T& operator[](std::size_t pos)
        {
                auto                        lower = FindPartition(pos);
                return std::get<0>(*lower)->operator[](pos - std::get<2>(*lower));
        }

        /// Gets const element at index without range check
        const T& operator[](std::size_t pos) const
        {
                auto                        lower = FindPartition(pos);
                return std::get<0>(*lower)->operator[](pos - std::get<2>(*lower));
        }

        /// Access the last element.
        T& back() { return m_partitions.back().back(); }

        /// Access the last element.
        const T& back() const { return m_partitions.back().back(); }

        /// Access the last element.
        T& back(std::size_t partitionId) { return m_partitions[partitionId].back(); }

        /// Access the last element.
        const T& back(std::size_t partitionId) const { return m_partitions[partitionId].back(); }

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
        virtual void Finalize()
        {
                std::size_t searchIndex = 0;
                for (auto it = m_partitions.begin(); it != m_partitions.end(); ++it) {
                        std::size_t sum = searchIndex + it->size();
                        m_prefixSums.emplace_back(it, sum, searchIndex);
                        //                        it->Finalize();
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

        /// Find in which partition pos lies
        typename std::vector<partItIndex>::const_iterator FindPartition(std::size_t pos) const
        {
                if (!m_finalized) {
                        throw Exception("Must be finalized");
                }
                return std_::lower_bound(m_prefixSums.cbegin(), m_prefixSums.cend(), pos,
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
