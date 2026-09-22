import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


# ============================================================
# C(i): Key Comparisons vs Array Size
# ============================================================

def plot_size_vs_comparisons(csv_filename):
    df = pd.read_csv(csv_filename)

    plt.figure(figsize=(10, 6))

    # Empirical results
    plt.plot(
        df['Array_Size'],
        df['Comparisons'],
        marker='o',
        linestyle='-',
        label='Empirical Key Comparisons'
    )

    # Theoretical O(n log n) curve
    n = df['Array_Size'].to_numpy()
    theoretical = n * np.log2(n)

    # Scale theoretical curve so its magnitude can be compared
    # visually with the empirical comparison counts
    scale_factor = (
        df['Comparisons'].iloc[-1] /
        theoretical[-1]
    )

    theoretical_scaled = theoretical * scale_factor

    plt.plot(
        n,
        theoretical_scaled,
        linestyle='--',
        label='Scaled n log₂(n)'
    )

    plt.title(
        'Key Comparisons vs. Array Size (Fixed S = 32)',
        fontsize=14
    )

    plt.xlabel('Array Size (n)', fontsize=12)
    plt.ylabel('Number of Key Comparisons', fontsize=12)

    plt.grid(True, linestyle='--', alpha=0.5)
    plt.legend()

    plt.tight_layout()

    plt.savefig(
        'graph_size_vs_comparisons.png',
        dpi=300
    )

    plt.show()

    print("Saved: graph_size_vs_comparisons.png")


# ============================================================
# C(ii): Effect of Threshold S
# Overall Trend + Detailed Staircase
# ============================================================

def plot_s_vs_comparisons(
    overall_csv_filename,
    staircase_csv_filename
):
    overall_df = pd.read_csv(overall_csv_filename)
    staircase_df = pd.read_csv(staircase_csv_filename)

    fig, (ax1, ax2) = plt.subplots(
        1,
        2,
        figsize=(15, 6)
    )

    # --------------------------------------------------------
    # LEFT: Overall trend
    # --------------------------------------------------------

    ax1.plot(
        overall_df['S_Value'],
        overall_df['Comparisons'],
        marker='o',
        linestyle='-'
    )

    ax1.set_title(
        'Overall Trend (S = 2 to 512)',
        fontsize=13
    )

    ax1.set_xlabel('Threshold S', fontsize=11)
    ax1.set_ylabel(
        'Number of Key Comparisons',
        fontsize=11
    )

    ax1.grid(
        True,
        linestyle='--',
        alpha=0.5
    )

    # --------------------------------------------------------
    # RIGHT: Detailed staircase
    # --------------------------------------------------------

    ax2.step(
        staircase_df['S_Value'],
        staircase_df['Comparisons'],
        where='post',
        linewidth=2
    )

    ax2.scatter(
        staircase_df['S_Value'],
        staircase_df['Comparisons'],
        s=10
    )

    ax2.set_title(
        'Detailed Staircase (S = 1 to 128)',
        fontsize=13
    )

    ax2.set_xlabel('Threshold S', fontsize=11)
    ax2.set_ylabel(
        'Number of Key Comparisons',
        fontsize=11
    )

    ax2.grid(
        True,
        linestyle='--',
        alpha=0.5
    )

    fig.suptitle(
        'Effect of Threshold S on Key Comparisons '
        '(Fixed n = 1,000,000)',
        fontsize=15
    )

    plt.tight_layout()

    plt.savefig(
        'graph_s_vs_comparisons.png',
        dpi=300,
        bbox_inches='tight'
    )

    plt.show()

    print("Saved: graph_s_vs_comparisons.png")


# ============================================================
# Part D: Original Merge Sort vs Hybrid Sort
# ============================================================

def plot_showdown(csv_filename):
    df = pd.read_csv(csv_filename)

    fig, (ax1, ax2) = plt.subplots(
        1,
        2,
        figsize=(12, 5)
    )

    # --------------------------------------------------------
    # Key comparisons
    # --------------------------------------------------------

    bars1 = ax1.bar(
        df['Algorithm'],
        df['Comparisons']
    )

    ax1.set_title(
        'Key Comparisons (10 Million Integers)'
    )

    ax1.set_ylabel('Comparisons')

    ax1.bar_label(
        bars1,
        labels=[
            f'{x:,.0f}'
            for x in df['Comparisons']
        ],
        padding=3,
        fontweight='bold'
    )

    # --------------------------------------------------------
    # CPU time
    # --------------------------------------------------------

    bars2 = ax2.bar(
        df['Algorithm'],
        df['CPU_Time_ms']
    )

    ax2.set_title(
        'CPU Time (10 Million Integers)'
    )

    ax2.set_ylabel('Time (ms)')

    # Start at zero so the visual comparison is fair
    ax2.set_ylim(
        0,
        df['CPU_Time_ms'].max() * 1.15
    )

    ax2.bar_label(
        bars2,
        labels=[
            f'{x:,.0f} ms'
            for x in df['CPU_Time_ms']
        ],
        padding=3,
        fontweight='bold'
    )

    plt.tight_layout()

    plt.savefig(
        'graph_10m_showdown.png',
        dpi=300
    )

    plt.show()

    print("Saved: graph_10m_showdown.png")


# ============================================================
# MAIN
# ============================================================

if __name__ == "__main__":

    print("Starting Data Analysis...")

    # C(i)
    try:
        plot_size_vs_comparisons(
            'data/experiment1_fixed_s.csv'
        )
    except FileNotFoundError:
        print(
            "Waiting on 'experiment1_fixed_s.csv'..."
        )

    # C(ii)
    try:
        plot_s_vs_comparisons(
            'data/experiment2_fixed_n.csv',
            'data/experiment2_staircase.csv'
        )
    except FileNotFoundError:
        print(
            "Waiting on C(ii) experiment files..."
        )

    # Part D
    try:
        plot_showdown(
            'data/experiment3.csv'
        )
    except FileNotFoundError:
        print(
            "Waiting on 'experiment3.csv'..."
        )
