from pathlib import Path
import csv
import matplotlib.pyplot as plt

# Find the CSV beside this script.
folder = Path(__file__).resolve().parent

with open(folder / "matrix_results.csv", newline="") as file:
    rows = list(csv.DictReader(file))

for row in rows:
    row["V"] = int(row["V"])
    row["E"] = int(row["E"])
    row["Average_Time_ms"] = float(row["Average_Time_ms"])


def plot_results(data, x_column, title, filename, quadratic=False):
    data = sorted(data, key=lambda row: row[x_column])

    x = [row[x_column] for row in data]
    times = [row["Average_Time_ms"] for row in data]

    fig, ax = plt.subplots(figsize=(8, 5))

    ax.plot(x, times, "o-", label="Measured average runtime")

    if quadratic:
        # Reference curve scaled to the final measurement.
        # This is a visual comparison, not a theoretical prediction in ms.
        scale = times[-1] / (x[-1] ** 2)
        reference = [scale * value**2 for value in x]

        ax.plot(
            x,
            reference,
            "--",
            label="V² reference scaled to final measurement"
        )

    ax.set_title(title)
    ax.set_xlabel(
        "Number of vertices |V|"
        if x_column == "V"
        else "Number of edges |E|"
    )
    ax.set_ylabel("Average runtime over 5 trials (ms)")
    ax.grid(True, alpha=0.3)
    ax.legend()

    fig.tight_layout()
    output = folder / filename
    fig.savefig(output, dpi=300)
    plt.close(fig)

    print(f"Saved: {output.name}")


# Experiment 1: fixed V, varying E.
plot_results(
    [row for row in rows if row["Filename"].startswith("graph_V1000_E")],
    "E",
    "Matrix + Array: Varying Edges at |V| = 1,000",
    "matrix_fixed_v.png"
)

# Experiment 2: sparse graphs, varying V.
plot_results(
    [row for row in rows if row["Filename"].startswith("graph_sparse_")],
    "V",
    "Matrix + Array: Sparse Graphs (|E| = 5|V|)",
    "matrix_sparse.png",
    quadratic=True
)

# Experiment 3: dense graphs, varying V.
plot_results(
    [row for row in rows if row["Filename"].startswith("graph_dense_")],
    "V",
    "Matrix + Array: Dense Graphs (|E| = |V|(|V| − 1)/2)",
    "matrix_dense.png",
    quadratic=True
)
