#credit to chat gpt for making this grapher

import matplotlib
matplotlib.use("Agg")  # Use non-interactive backend (no GUI)

import matplotlib.pyplot as plt

# Change this to your filename
filename = "uniformC1Output.txt"

# Read numbers from the file
with open(filename, "r") as f:
    numbers = [float(line.strip()) for line in f if line.strip()]

# Determine bin edges so that each bin has width = 1
min_val = int(min(numbers))
max_val = int(max(numbers))
bins = range(min_val, max_val + 2)  # +2 so last bin includes max value

# Bigger figure, and bars narrower to create space between them
plt.figure(figsize=(10, 6))  # width, height in inches
plt.hist(numbers, bins=bins, edgecolor="black", align="left", rwidth=0.7)


# Labels and title
plt.xlabel("Value")
plt.ylabel("Frequency")
plt.title("Histogram (bin size = 1)")

# Save to file
plt.savefig("histogram.png", dpi=300, bbox_inches="tight")
print("Histogram saved to histogram.png")