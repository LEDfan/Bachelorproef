import numpy
import sys
import matplotlib.pyplot as plt

a = []
plt.hist(a, bins="auto")
print("The mean is:", numpy.mean(a))
print("The standard deviation is:", numpy.std(a))
print("The variance is:", numpy.var(a))
plt.title("Histogram")
plt.xlabel('Amount of people infected at the end')
plt.ylabel('Amount of occurences')

plt.show()

import numpy as np 
import pylab 
import scipy.stats as stats
#QQ
stats.probplot(a, dist="norm", plot=pylab)
pylab.show()

#Boxplot
plt.figure()
plt.title("Boxplot")
plt.xlabel('Amount of people infected at the end')
plt.ylabel('Amount of occurences')
plt.boxplot(a, 0, 'rs', 0, 0.75)
plt.show()
