import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import ListedColormap
from matplotlib.patches import Patch
import pandas as pd

# -------------------------------
# CONFIGURATION
# -------------------------------
df = pd.read_csv("test\\planning_grid.csv")
GRID_X, GRID_Y = df["x"].iloc[0], df["y"].iloc[0]

df = pd.read_csv("test\\planning_start.csv")
start = list(zip(df["x"], df["y"]))

df = pd.read_csv("test\\planning_end.csv")
end = list(zip(df["x"], df["y"]))

# blockers
df = pd.read_csv("test\\planning_blockers.csv")
blockers = list(zip(df["x"], df["y"]))

# A* path
df = pd.read_csv("test\\planning_path.csv")
path = list(zip(df["x"], df["y"]))

# open
df = pd.read_csv("test\\planning_open.csv")
open = list(zip(df["x"], df["y"]))

# closed
df = pd.read_csv("test\\planning_closed.csv")
closed = list(zip(df["x"], df["y"]))

colorsList = [
    "white",   # 0 empty
    "black",   # 1 wall
    "orange",  # 2 open
    "blue",    # 3 closed
    "green",   # 4 path
]
cmap = ListedColormap(colorsList)

# -------------------------------
# BUILD GRID
# -------------------------------
grid = np.zeros((GRID_X, GRID_Y))

for (x, y) in blockers:
    grid[x][y] = 1  # wall

for (x, y) in open:
    grid[x][y] = 2   # open

for (x, y) in closed:
    grid[x][y] = 3   # closed

for (x, y) in path:
    grid[x][y] = 4   # path


# -------------------------------
# VISUALIZE
# -------------------------------

plt.figure(figsize=(6, 6))
plt.imshow(grid.T, origin="lower", cmap=cmap)

startColor = "yellow"
plt.scatter(start[0], start[0], color=startColor)
endColor = "red"
plt.scatter(end[0], end[0], color=endColor)

plt.grid(True)
plt.xticks(np.arange(GRID_X) + 0.5, labels="")
plt.yticks(np.arange(GRID_Y) + 0.5, labels="")

plt.gca().set_xticks(np.arange(GRID_X), labels=np.arange(GRID_X), minor=True)
plt.gca().set_yticks(np.arange(GRID_Y), labels=np.arange(GRID_Y), minor=True)

plt.title("A* Grid Visualization")
plt.xlabel("X")
plt.ylabel("Y")

legend_labels = {
    0: "Empty",
    1: "Wall",
    2: "open",
    3: "closed",
    4: "path",
}

# Create legend manually
legend_handles = [
    Patch(color=cmap(i), label=legend_labels[i])
    for i in legend_labels
]
legend_handles.append(Patch(color=startColor, label="start"))
legend_handles.append(Patch(color=endColor, label="end"))

plt.legend(handles=legend_handles, bbox_to_anchor=(1.05, 1), loc='upper left')
plt.tight_layout()
plt.show()
