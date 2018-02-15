import pystride
from pystride.Simulation import Simulation

# Set the workspace (default = .)
pystride.workspace = "simulations"

# Configure simulation
simulation = Simulation()
simulation.loadRunConfig("config/run_default.xml")

# Create forks with varying parameters
for r0 in [5,15] + list(range(20, 60, 10)):
    # Create fork
    fork = simulation.fork('r0-' + str(r0))

    fork.runConfig.setParameter("r0", r0)

# Run all forks
simulation.runForks()
