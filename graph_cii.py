import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("cii_results.csv")

n = 1000000

# ------------------------------------------------
# GRAPH 1:
# Experimental Key Comparisons vs Theoretical
# ------------------------------------------------

df["Theoretical"] = (
    n * np.log2(n / df["S"])
    + (n * df["S"]) / 4
)

plt.figure(figsize=(9, 5))

plt.plot(
    df["S"],
    df["Key Comparisons"],
    marker="o",
    markersize=3,
    label="Hybrid Merge Sort"
)

plt.plot(
    df["S"],
    df["Theoretical"],
    linestyle="--",
    label="Theoretical C(n,S)"
)

plt.xlabel("Threshold S")
plt.ylabel("Key Comparisons")
plt.title("C(ii): Key Comparisons vs Threshold S")

plt.legend()
plt.grid(True)
plt.tight_layout()

plt.savefig(
    "cii_key_comparisons.png",
    dpi=300
)

plt.show()


# ------------------------------------------------
# GRAPH 2:
# CPU Time vs S
# ------------------------------------------------

plt.figure(figsize=(9, 5))

plt.plot(
    df["S"],
    df["CPU Time (ms)"],
    marker="s",
    markersize=4,
    label="Hybrid Merge Sort"
)

plt.xlabel("Threshold S")
plt.ylabel("CPU Time (ms)")
plt.title("C(ii): CPU Time vs Threshold S")

plt.legend()
plt.grid(True)
plt.tight_layout()

plt.savefig(
    "cii_cpu_time.png",
    dpi=300
)

plt.show()
