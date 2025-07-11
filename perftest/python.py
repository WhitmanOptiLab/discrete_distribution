import os
import glob
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np

# Get all result files and sort by level
files = sorted(glob.glob("results_*.csv"), key=lambda x: int(x.split("_")[1].split(".")[0]))

levels = []
data_by_test = {}

for file in files:
    level = int(file.split("_")[1].split(".")[0])
    levels.append(level)

    df = pd.read_csv(file, header=0)
    average_row = df.iloc[-1]

    for col in df.columns:
        data_by_test.setdefault(col.strip(), []).append(float(average_row[col]))

# Plotting
plt.figure(figsize=(12, 7))

# Create a colormap with N distinct colors
num_lines = len(data_by_test)
colors = cm.get_cmap('tab20', num_lines)  # or 'tab10', 'nipy_spectral', etc.

for i, (test_name, times) in enumerate(data_by_test.items()):
    plt.plot(levels, times, marker='o', label=test_name, color=colors(i))

plt.xlabel("Number of Weights (Level)")
plt.ylabel("Average Time (seconds)")
plt.title("Performance of Weighted Random Selection Methods")
plt.legend()
plt.grid(True)
plt.xscale("log")
plt.tight_layout()
plt.show()
