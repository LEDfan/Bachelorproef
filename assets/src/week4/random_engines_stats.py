from __future__ import print_function
import sys
import matplotlib.pyplot as plt
import scipy.stats

data = {}
for line in open(sys.argv[1]):
    line = line.strip()
    engine, mean, sigma, pts = line.split(" ", 3)
    data[engine] = (float(mean), float(sigma), map(float, pts[1:-1].split(", ")))

name_base = sys.argv[2]

for engine in sorted(data):
    filename = "{}_boxplot_{}.png".format(name_base, engine)
    plt.figure()
    plt.title("Boxplot")
    plt.xlabel('Amount of people infected at the end')
    plt.ylabel('Amount of occurences')
    plt.boxplot(data[engine][2], 0, 'rs', 0, 0.75)
    plt.savefig(filename)

    filename = "{}_qq_{}.png".format(name_base, engine)
    plt.figure()
    scipy.stats.probplot(data[engine][2], dist="norm", plot=plt)
    plt.savefig(filename)

    filename = "{}_histo_{}.png".format(name_base, engine)
    plt.figure()
    plt.xlabel("Infected count")
    plt.ylabel("Number of occurrences")
    plt.hist(data[engine][2])
    plt.savefig(filename)

    p = scipy.stats.shapiro(data[engine][2])[1]
    mean = data[engine][0]
    sigma = data[engine][1]
    print("Acceptable range for {}, engine {}: [{:d}, {:d}]".format(name_base, engine, int(round(mean - 2*sigma)), int(round(mean + 2*sigma))))
    print("Normality of {}, engine {}: p = {:0.4f}".format(name_base, engine, p))
    print()

for engine in sorted(data):
    for e2 in sorted(data):
        if e2 <= engine: continue
        p = scipy.stats.ttest_rel(data[engine][2], data[e2][2])[1]
        print("Comparison of {} and {}: equal mean with p = {:0.4f}".format(engine, e2, p))
        p = scipy.stats.levene(data[engine][2], data[e2][2])[1]
        print("Comparison of {} and {}: equal sigma with p = {:0.4f}".format(engine, e2, p))
        print()

