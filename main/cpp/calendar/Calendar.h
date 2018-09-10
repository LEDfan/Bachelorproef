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
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J, Avé T
 */

/**
 * @file
 * Header file for the Calendar class.
 */

#include <boost/property_tree/ptree.hpp>
#include <date/date.h>

#include <algorithm>
#include <cstdlib>
#include <memory>
#include <vector>

namespace stride {

/**
 * Class that keeps track of the 'state' of simulated world.
 * E.g. what day it is, holidays, quarantines, ...
 */
class Calendar
{
public:
        /// Constructor
        explicit Calendar(const boost::property_tree::ptree& configPt);

        /// Advance the simulated calendar by one day.
        void AdvanceDay();

        /// Current day of the month in the simulated calendar.
        std::size_t GetDay() const { return static_cast<unsigned int>(m_date.day()); }

        /// Current day of the week (0 (Sunday), ..., 6 (Saturday)) in the simulated calendar.
        std::size_t GetDayOfTheWeek() const
        {
                return static_cast<unsigned>(static_cast<date::year_month_weekday>(m_date).weekday());
        }

        /// Current month in the simulated calendar.
        std::size_t GetMonth() const { return static_cast<unsigned int>(m_date.month()); }

        /// Current simulated day since the start of the simulation.
        unsigned short int GetSimulationDay() const { return m_day; }

        /// Current year in the simulated calendar.
        std::size_t GetYear() const { return static_cast<int>(m_date.year()); }

        /// Check if it's a holiday
        bool IsHoliday() const { return (std::find(m_holidays.begin(), m_holidays.end(), m_date) != m_holidays.end()); }

        /// Check if it's a school holiday
        bool IsSchoolHoliday() const
        {
                return (std::find(m_school_holidays.begin(), m_school_holidays.end(), m_date) !=
                        m_school_holidays.end());
        }

        /// Check if it's the weekend
        bool IsWeekend() const { return (GetDayOfTheWeek() == 6 || GetDayOfTheWeek() == 0); }

private:
        ///
        void InitializeHolidays(const boost::property_tree::ptree& configPt);

        date::year_month_day ConvertFromString(const std::string& day);

private:
        date::year_month_day              m_date;            ///< Current simulated date.
        unsigned short int                m_day;             ///< Current day since start of simulation.
        std::vector<date::year_month_day> m_holidays;        ///< Vector of general holidays
        std::vector<date::year_month_day> m_school_holidays; ///< Vector of school holidays
};

} // namespace stride
