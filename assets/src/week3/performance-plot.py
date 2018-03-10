from __future__ import division
from matplotlib import pyplot as plt

data = """
424.26398253440857
412.8457384109497
406.3430984020233
410.96028757095337
414.76421093940735
404.68358635902405
411.8800308704376
399.4433858394623
410.85408067703247
400.93526577949524
410.2799000740051
410.11667227745056
410.9897406101227
415.42817521095276
405.3887548446655
412.0431909561157
410.4708659648895
402.75837111473083
409.905606508255
398.96197867393494
405.3623435497284
406.8555784225464
409.3024830818176
418.8289692401886
414.4332432746887
411.9066433906555
411.2791726589203
413.42382192611694
404.75378370285034
413.02616143226624
415.18087553977966
412.6926236152649
""".strip().splitlines()
data = list(map(lambda x: float(x)/15, data))
plt.plot(list(range(1,33)), data)
plt.scatter(list(range(1,33)), data, marker="x", color="red")
plt.xlabel("Threads")
plt.ylabel("Runtime (s)")
plt.show()

plt.figure()
plt.title("Boxplot")
plt.xlabel('Runtime (s)')
plt.ylabel('Amount of occurences')
plt.boxplot(data, 0, 'rs', 0, 0.75)
plt.show()
