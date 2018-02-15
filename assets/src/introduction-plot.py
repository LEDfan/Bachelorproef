from __future__ import division
from matplotlib import pyplot as plt

data = """
1 1369    600000
5 2320 600000
10 6011   600000
15 15217 600000
20 32180  600000
30 81433  600000
40 108901 600000
50 116793 600000
60 119312 600000
""".strip().splitlines()
data = map(lambda l: map(float, l.strip().split()), data)

data = (map(lambda l: l[0], data), map(lambda l: l[1] / l[2], data))
plt.plot(*data)
plt.scatter(*data, marker="x", color="red")
plt.xlabel("$R_0$")
plt.ylabel("Attack rate")
locs, _ = plt.yticks()
plt.yticks(locs, ["%.1f%%" % (100*y) for y in locs])
plt.ylim((0, plt.ylim()[1]))
plt.xkcd()
plt.show()
