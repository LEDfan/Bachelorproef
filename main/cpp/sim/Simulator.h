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
 *  Copyright 2017, 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Header for the Simulator class.
 */

#include "contact/AgeContactProfiles.h"
#include "contact/ContactLogMode.h"
#include "contact/TransmissionProfile.h"
#include "pool/ContactPoolSys.h"
#include "sim/python/SimulatorObserver.h"
#include "sim/python/Subject.h"
#include "util/RNManager.h"

#include <boost/property_tree/ptree.hpp>

namespace spdlog {
class logger;
}

namespace stride {

class Calendar;
class Population;

/**
 * Simulator can time step and reveal some of the key data.
 * The Subject base class used for the interaction with the python environment only.
 */
class Simulator : public python::Subject<unsigned int, python::SimulatorObserver>
{
public:
        /// Default constructor for empty Simulator.
        Simulator();

        /// Check if the simulator is operational.
        bool IsOperational() const { return m_operational; }

        /// Caledar associated with simulated world. Represents date/simulated day of
        /// last TimeStep completed (it is incremented at the very end of TimeStep).
        std::shared_ptr<Calendar> GetCalendar() const { return m_calendar; }

        /// Get the contact logger.
        std::shared_ptr<spdlog::logger> GetContactLogger() { return m_contact_logger; }

        /// The ContactPoolSys of the simulator.
        ContactPoolSys& GetContactPoolSys() { return m_pool_sys; }

        /// The ContactPoolSys of the simulator.
        const ContactPoolSys& GetContactPoolSys() const { return m_pool_sys; }

        /// Get the disease profile.
        const TransmissionProfile& GetDiseaseProfile() const { return m_disease_profile; }

        /// Get the population.
        std::shared_ptr<Population> GetPopulation() { return m_population; }

        /// Get the random number manager.
        util::RNManager& GetRNManager() { return m_rn_manager; }

        /// Run one time step, computing full simulation (default) or only index case.
        void TimeStep();

private:
        /// Update the contacts in the given contactpools.
        template <ContactLogMode::Id log_level, typename local_information_policy, bool track_index_case = false>
        void UpdatePools();

private:
        boost::property_tree::ptree     m_config_pt;        ///< Configuration property tree
        ContactLogMode::Id              m_contact_log_mode; ///< Specifies contact/transmission logging mode.
        std::shared_ptr<spdlog::logger> m_contact_logger;   ///< Logger for contact/transmission.
        AgeContactProfiles              m_contact_profiles; ///< Contact profiles w.r.t age.
        TransmissionProfile             m_disease_profile;  ///< Profile of disease.
        unsigned int                    m_num_threads;      ///< The number of (OpenMP) threads.
        bool                            m_track_index_case; ///< General simulation or tracking index case.

        std::shared_ptr<Calendar> m_calendar;    ///< Management of calendar.
        bool                      m_operational; ///< False when invalid disease profile is specified.
        util::RNManager           m_rn_manager;  ///< Random numbere generation management.

private:
        ///< Last simulated day; in TimeStep it is the currently simulating day i.e. m_sim_day is
        ///< incremented at the beginning of TimeStep and should be used with coution inside TimeStep.
        unsigned int m_sim_day;

private:
        std::shared_ptr<Population> m_population;               ///< Pointer to the Population.
        ContactPoolSys              m_pool_sys;                 ///< Holds vector of ContactPool of different types.
        std::string                 m_local_information_policy; ///< Local information name.

private:
        friend class SimulatorBuilder;
        friend class PopPoolBuilder;
};

} // namespace stride
