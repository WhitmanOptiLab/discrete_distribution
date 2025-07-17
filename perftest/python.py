import os
import glob
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
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

# Define families and their base colors (as RGB tuples)
family_colors_rgb = {
    "fenwick": mcolors.to_rgb("red"),
    "discrete": mcolors.to_rgb("blue"),
    "leafsum": mcolors.to_rgb("green"),
    "wrs": mcolors.to_rgb("orange"),
    "heap": mcolors.to_rgb("purple"),
}

families = list(family_colors_rgb.keys())
family_to_tests = {fam: [] for fam in families}
others = []

# Categorize each test by family
for test_name in data_by_test.keys():
    found = False
    for fam in families:
        if fam in test_name.lower():
            family_to_tests[fam].append(test_name)
            found = True
            break
    if not found:
        others.append(test_name)

# Define distribution-based markers
distribution_markers = {
    "uniform": "o",
    "normal": "s",
    "weibull": "D",
}
default_marker = "x"

# Helper to blend colors toward white for lighter variants
def get_variant_color(base_rgb, index, total):
    # Blend from 0% to 50% toward white
    blend_ratio = 0.0 + 0.7 * (index / max(total - 1, 1))
    return tuple(blend_ratio * 1.0 + (1 - blend_ratio) * c for c in base_rgb)


# Start plotting
plt.figure(figsize=(12, 7))

# Plot tests from known families
for fam in families:
    tests = family_to_tests[fam]
    base_rgb = family_colors_rgb[fam]
    for idx, test_name in enumerate(tests):
        times = data_by_test[test_name]

        linestyle = '--' if "static" in test_name.lower() else '-'

        marker = default_marker
        for dist, sym in distribution_markers.items():
            if dist in test_name.lower():
                marker = sym
                break

        color = get_variant_color(base_rgb, idx, len(tests))

        plt.plot(
            levels,
            times,
            label=test_name,
            color=color,
            linestyle=linestyle,
            marker=marker,
            markersize=6
        )

# Plot tests from unknown categories
for idx, test_name in enumerate(others):
    times = data_by_test[test_name]
    linestyle = '--' if "static" in test_name.lower() else '-'

    marker = default_marker
    for dist, sym in distribution_markers.items():
        if dist in test_name.lower():
            marker = sym
            break

    color = (0.2, 0.2, 0.2)  # Dark gray for unknown category

    plt.plot(
        levels,
        times,
        label=test_name,
        color=color,
        linestyle=linestyle,
        marker=marker,
        markersize=6
    )
legend = plt.legend(loc="best", fancybox=True, shadow=True)
lines = plt.gca().get_lines()

# Map legend items (handles) to plotted lines
lined = {}

for legline, origline in zip(legend.get_lines(), lines):
    legline.set_picker(5)  # 5 points tolerance
    lined[legline] = origline

for legtext, origline in zip(legend.get_texts(), lines):
    legtext.set_picker(True)
    lined[legtext] = origline

def on_pick(event):
    artist = event.artist
    if artist in lined:
        origline = lined[artist]
        visible = not origline.get_visible()
        origline.set_visible(visible)
        # Fade legend item
        alpha = 1.0 if visible else 0.2
        artist.set_alpha(alpha)
        plt.gcf().canvas.draw_idle()

plt.gcf().canvas.mpl_connect('pick_event', on_pick)


plt.xlabel("Number of Weights (Level)")
plt.ylabel("Average Time (seconds)")
plt.title("Performance of Weighted Random Selection Methods")
plt.grid(True)
plt.xscale("log")
plt.tight_layout()
plt.show()