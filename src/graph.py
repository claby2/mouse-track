import matplotlib.pyplot as plt, csv, optparse
from numpy import genfromtxt

parser = optparse.OptionParser();
parser.add_option('-s', action = "store_true", dest = "save", default = False, help = "save plot as image")
options, args = parser.parse_args()

data_file_path = '../data.csv'

# Get data from CSV
data = genfromtxt(data_file_path, delimiter=',')

x = [i[0] for i in data]
y = [i[1] for i in data]

# Set screen dimensions from CSV meta data
def get_screen_dimensions():
    with open(data_file_path, newline='') as f:
        reader = csv.reader(f)
        return(next(reader)[1], next(reader)[1])

screen_dimensions = get_screen_dimensions()
screen_width = int(screen_dimensions[0])
screen_height = int(screen_dimensions[1])

# Delete meta data from data to be plotted which is nan
x = x[1:]
y = y[1:]

# Plot x and y and set axis
plt.plot(x, y)
axes = plt.gca()
axes.set_xlim([0, screen_width])
axes.set_ylim([0, screen_height])
plt.axis('off')
if(options.save):
    plt.savefig("../graph.png")
plt.show()