Performance Analysis plan
===

- Starting from the `run_default.xml` config file, we intend to generate variations on separate parameters to try and find a regression that could serve as an indication of the influence on the run time with respect to that parameter. We can then create a formula based on these parameters and corresponding regressions, which will result in an estimated run time when calculated with their concrete values.

- Split into parts and analyze influence indivually and combined:

    + Model population
    + Setup (e.g. create simulation, create population)
    + Simulation

- Some computational and theoretical parameters we suspect might influence run time:

    + Population size
    + $R_0$
    + Seeding rate
    + Amount of commutes / multi-region tourism
    + Disease (some other parameters could serve as proxy for disease properties)
    + Immunity/Vaccinations
    + Random number generator
    + Amount of contact pools ~ Population size / size of contact pools
    + Amount of data collected/processed ~ viewers

- Some hardware characteristics that are probably interesting to look at:

    + SSD vs HDD
    + RAM speed
    + Cache size
    + Number of cache levels
    + Number of threads
    + Processor speed
    + File system cache
    + Swap


Tools we might use
---

 - perf: analysing function runtime and hardware counters
 - valgrind callgrind: analysing function runtime and call graph
 - valgrind massif: profile heap usage
 - `stride::util::Stopwatch`
 - myhayai: macro benchmarking
