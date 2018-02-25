import itertools
import random
import numpy
import shutil
import pystride
from pystride.Simulation import Simulation

# Set the workspace (default = .)
pystride.workspace = "simulations"

# Configure simulation
simulation = Simulation()
simulation.loadRunConfig("config/run_default.xml")
simulation.runConfig.setParameter("num_days", 30)
simulation.runConfig.setParameter("r0", 2.0)
simulation.runConfig.setParameter("r0", 2.0)
simulation.runConfig.setParameter("rng_seed", 2015)
simulation.runConfig.setParameter("seeding_rate", 0.0009)
simulation.runConfig.setParameter("immunity_profile", "Random")
simulation.runConfig.setParameter("immunity_rate", 0.0)
simulation.runConfig.setParameter("vaccine_profile", "None")
simulation.runConfig.setParameter("disease_config_file", "disease_influenza")
simulation.runConfig.setParameter("output_prefix", "test")
simulation.runConfig.setParameter("log_level", "None")

results = []
random.seed(0)
outputfile = open('out.txt', 'w')
i = 0
while(True):
    i += 1
    seed = random.randint(0, 1000000000)
    forkDefault = simulation.fork('rng_reed' + str(seed))
    forkDefault.runConfig.setParameter("rng_seed", seed);
    forkDefault.run()
    result = forkDefault.runner.GetSimulator().GetPopulation().GetInfectedCount()
    print(result)
    results.append(result)
    outputfile.write(str(result) + "\n")
    outputfile.flush()
    shutil.rmtree("simulations/test")
    simulation.delForks()

    print("SIM " + str(i))
    print("Mean")
    print(numpy.mean(results))

    print("Dev:")
    print(numpy.std(results))
    print(results)

# immunityRate = simulation.fork('immunityRate')
# immunityRate.runConfig.setParameter("seeding_rate", (1 - 0.9991) / 100);
# immunityRate.runConfig.setParameter("immunity_rate", 0.9991);

# measles = simulation.fork('measles')
# measles.runConfig.setParameter("disease_config_file", "disease_measles.xml");
# measles.runConfig.setParameter("r0", 16);

# maximum = simulation.fork('maximum')
# maximum.runConfig.setParameter("disease_config_file", "disease_measles.xml");
# maximum.runConfig.setParameter("r0", 60);

