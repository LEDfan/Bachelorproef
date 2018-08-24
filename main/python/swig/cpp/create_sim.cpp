#include "create_sim.h"
#include <util/RnMan.h>
#include <util/RunConfigManager.h>

stride::Sim CreateSim(std::string configString)
{
        auto config_pt = stride::util::RunConfigManager::FromString(configString);

        stride::util::RnMan rnMan;
        rnMan.Initialize(stride::util::RnMan::Info{config_pt.get<std::string>("pop.rng_seed", "1,2,3,4"), "",
                                                   config_pt.get<unsigned int>("run.num_threads")});
        auto population = stride::Population::Create(config_pt, rnMan);

        // We need a version without a shared_ptr since it creates a segfault
        // which is hard to debug without being able to use GDB
        return stride::Sim(*stride::Sim::Create(config_pt, population, rnMan).get());
}
