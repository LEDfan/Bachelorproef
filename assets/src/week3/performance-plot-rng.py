from __future__ import division
import numpy as np
from matplotlib import pyplot as plt

data = {
"lgc64": 409.2489387989044,
"lgc64_shift": 415.4448504447937,
"mrg2": 417.16411685943604,
"mgr3": 426.9849667549133,
"yarn2": 420.879417181015,
"yarn3": 466.4946506023407
}

plt.figure()
plt.xlabel("RNG Engine")
plt.ylabel("Runtime (s)")
plt.bar(data.keys(), map(lambda x: float(data[x]), data.keys()), 0.35)
plt.ylim(ymin=300, ymax=max(data.values()) + 20)
plt.show()

