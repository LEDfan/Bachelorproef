import itertools
import random
import numpy
import shutil
import pystride
import time
import os
from pystride.Simulation import Simulation

# Set the workspace (default = .)
pystride.workspace = "simulations"

# Configure simulation
simulation = Simulation()
# simulation.loadRunConfig("config/run_default.xml")
simulation.runConfig.setParameter("rng_seed", 2015);
simulation.runConfig.setParameter("r0", 3.0);
simulation.runConfig.setParameter("seeding_rate", 0.0009);
simulation.runConfig.setParameter("seeding_age_min", 1);
simulation.runConfig.setParameter("seeding_age_max", 99);
simulation.runConfig.setParameter("immunity_profile", "Random");
simulation.runConfig.setParameter("immunity_rate", 0.0);
simulation.runConfig.setParameter("immunity_link_probability", 0);
simulation.runConfig.setParameter("vaccine_profile", "None");
simulation.runConfig.setParameter("population_file", "pop_flanders600.csv");
simulation.runConfig.setParameter("num_days", 30);
simulation.runConfig.setParameter("output_prefix", "test");
simulation.loadDiseaseConfig("data/disease_influenza.xml")
# simulation.runConfig.setParameter("disease_config_file", "disease_influenza.xml");
simulation.runConfig.setParameter("num_participants_survey", 10);
simulation.runConfig.setParameter("start_date", "2017-01-01");
simulation.runConfig.setParameter("holidays_file", "holidays_none.json");
simulation.runConfig.setParameter("age_contact_matrix_file", "contact_matrix_flanders_subpop.xml");
simulation.runConfig.setParameter("log_level", "None");
simulation.runConfig.setParameter("local_information_policy", "NoLocalInformation");
simulation.runConfig.setParameter("global_information_policy", "NoGlobalInformation");
simulation.runConfig.setParameter("belief_policy/name", "NoBelief");
simulation.runConfig.setParameter("behaviour_policy", "NoBehaviour");
simulation.runConfig.setParameter("generate_person_file", 1);

# Influenza A

# Influenza B
# simulation.runConfig.setParameter("seeding_rate", 0.0);

# Influenza C
# simulation.runConfig.setParameter("seeding_rate", (1 - 0.9991) / 100);
# simulation.runConfig.setParameter("immunity_rate", 0.9991);

# Measles 16
# simulation.runConfig.setParameter("disease_config_file", "disease_measles.xml");
# simulation.runConfig.setParameter("r0", 16);

# Measles 60
# simulation.runConfig.setParameter("disease_config_file", "disease_measles.xml");
# simulation.runConfig.setParameter("r0", 60);

out = open('times.txt', 'a')

results = []
random.seed(0)
startTime = time.time()
amtRuns = 15
for i in range(amtRuns):
	seed = random.randint(0, 1000000000)
	forkDefault = simulation.fork('rng_reed' + str(seed))
	forkDefault.runConfig.setParameter("rng_seed", seed);
	forkDefault.run()
	result = forkDefault.runner.GetSimulator().GetPopulation().GetInfectedCount()
	print(i, " of ", amtRuns)
elapsed = time.time() - startTime
out.write(str(elapsed) + '\n')
out.flush()
print("threads TIME NEEDED: ", elapsed)
