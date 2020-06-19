import matplotlib.pyplot as plt, numpy as np, sys
from numpy import genfromtxt

data = genfromtxt('data.csv', delimiter=',')

x = [i[0] for i in data]
y = [i[1] for i in data]

# Delete first line of csv from data to be plotted
del x[0]
del y[0]

plt.scatter(x, y)
plt.axis('off')
plt.show()