import os
import glob
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import matplotlib.gridspec as gridspec
from matplotlib.widgets import CheckButtons
import numpy as np

# Load data as before

files = glob.glob("resultsC5_*.csv")
contents=[]
names=[]
weightnums=[]
averages=[]



for file in files:
    fileName=file.rsplit("/",1)[-1]
    fileName=fileName.split("_",1)[-1]
    fileName=fileName.rsplit(".",1)[0]
    fileName=fileName.strip()
    names.append(fileName)
    openFile=open(file,"r")
    contents.append(openFile.read().splitlines())
    openFile.close()
    

def csvLineToList(inputString):
    split = inputString.split(",")
    results=[]
    for i in range(len(split)):
        split[i]=split[i].strip()
        if (split[i]!=""):
            results.append(float(split[i]))
    return results

weightnums = csvLineToList(contents[0][0])

for test in contents:
    if csvLineToList(test[0]) != weightnums:
        print("headers don't match")

for test in contents:
    averages.append(csvLineToList(test[-1]))


# Create figure and axis
fig, ax = plt.subplots(figsize=(10, 6))
plt.subplots_adjust(right=.7)

# Plot lines and store references
line_objects = []
for name, y_values in zip(names, averages):
    print(weightnums)
    line, = ax.plot(weightnums, y_values, label=name, marker="o")
    line_objects.append((name, line))

# Create the CheckButtons
ax_legend = plt.axes([.75, 0.8, 0.23, 0.2])  # x, y, width, height
labels, lines = zip(*line_objects)
check = CheckButtons(ax_legend, labels, [True]*len(labels))

# Make labels the same color as their lines
for label_text, line in zip(check.labels, lines):
    label_text.set_color(line.get_color())

# Toggle function
def toggle_line(label):
    index = labels.index(label)
    lines[index].set_visible(not lines[index].get_visible())
    plt.draw()

check.on_clicked(toggle_line)

# Labels, title, grid
ax.set_xscale("log")
ax.set_xlabel("weightnum")
ax.set_ylabel("time")
ax.set_title("C5")
ax.grid(True)

plt.show()


