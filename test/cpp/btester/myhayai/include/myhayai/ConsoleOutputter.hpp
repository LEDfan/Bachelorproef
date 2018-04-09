#pragma once
/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Kuylen E, Willem L, Broeckhove J
 *
 *  This software has been altered form the hayai software by Nick Bruun.
 *  The original copyright, to be found in the directory two levels higher
 *  still aplies.
 */
/**
 * @file
 * Header file for ConsoleOutputter.
 */

#include "Console.hpp"
#include "Outputter.hpp"

#include <iomanip>

namespace myhayai {

/// Console outputter. Prints the result to standard output.
class ConsoleOutputter : public Outputter
{
public:
        /// Initialize console outputter.
        /// @param stream Output stream. Must exist for the entire duration of
        /// the outputter's use.
        explicit ConsoleOutputter(std::ostream& stream = std::cout) : m_stream(stream) {}

        ///
        void Begin(const std::size_t& enabledCount, const std::size_t& disabledCount) override;

        ///
        void End(const std::size_t& executedCount, const std::size_t& disabledCount) override;

        void BeginOrSkipTest(const std::string& fixtureName, const std::string& testName,
                             const InfoFactory& infoFactory, const std::size_t& runsCount, const bool skip);

        void BeginTest(const std::string& fixtureName, const std::string& testName, const InfoFactory& infoFactory,
                       const std::size_t& runsCount) override;

        void SkipDisabledTest(const std::string& fixtureName, const std::string& testName,
                              const InfoFactory& infoFactory, const std::size_t& runsCount) override;

        void EndTest(const std::string& fixtureName, const std::string& testName, const InfoFactory& infoFactory,
                     const TestResult& result) override;

private:
        std::ostream& m_stream;
};

} // namespace myhayai
