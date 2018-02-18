from __future__ import division
from matplotlib import pyplot as plt

data = """
1 57504 60000
5 58507 60000
10 57971 60000
15 56439 60000
20 56274 60000
30 54655 60000
40 52902 60000
50 42283 60000
60 31841 60000
""".strip().splitlines()
data = map(lambda l: map(float, l.strip().split()), data)

data = (map(lambda l: l[0], data), map(lambda l: l[1] / l[2], data))
plt.plot(*data)
plt.scatter(*data, marker="x", color="red")
plt.xlabel("$R_0$")
plt.ylabel("Attack rate")
locs, _ = plt.yticks()
plt.yticks(locs, ["%.1f%%" % (100*y) for y in locs])
plt.ylim((0.5, 1.0))
plt.show()
