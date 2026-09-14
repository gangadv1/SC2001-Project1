import pandas as pd
import matplotlib.pyplot as plt

def plot_size_vs_comparisons(csv_filename):

    df = pd.read_csv(csv_filename)

    # create the plot
    plt.figure(figsize=(10, 6))
    plt.plot(df['Array_Size'], df['Comparisons'], marker='o', linestyle='-', color='b')

    # formatting
    plt.title('Number of Key Comparisons vs. Array Size (Fixed S)', fontsize=14)
    plt.xlabel('Array Size (n)', fontsize=12)
    plt.ylabel('Number of Key Comparisons', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.7)

    plt.savefig('graph_size_vs_comparisons.png')
    plt.show()
    print("Saved: graph_size_vs_comparisons.png")

def plot_s_vs_comparisons(csv_filename):
    
    df = pd.read_csv(csv_filename)
    
    plt.figure(figsize=(10, 6))
    
    # changed to show the difference more significantly
    plt.plot(df['S_Value'].astype(str), df['Comparisons'], marker='s', linestyle='-', color='r')
    
    plt.title('Number of Key Comparisons vs. Threshold S (Fixed n = 1,000,000)', fontsize=14)
    plt.xlabel('Threshold (S)', fontsize=12)
    plt.ylabel('Number of Key Comparisons', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.7)
    
    plt.savefig('graph_s_vs_comparisons.png')
    plt.show()
    print("Saved: graph_s_vs_comparisons.png")

def plot_showdown(csv_filename):
    
    df = pd.read_csv(csv_filename)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))
    
    # chart 1: comparisons
    bars1 = ax1.bar(df['Algorithm'], df['Comparisons'], color=['#3498db', '#2ecc71'])
    ax1.set_title('Key Comparisons (10 Million Integers)')
    ax1.set_ylabel('Comparisons')
    
    # add exact numbers on top of the comparison bars (clearer)
    ax1.bar_label(bars1, labels=[f'{x:,.0f}' for x in df['Comparisons']], padding=3, fontweight='bold')
    
    # chart 2: CPU time
    bars2 = ax2.bar(df['Algorithm'], df['CPU_Time_ms'], color=['#e74c3c', '#f1c40f'])
    ax2.set_title('CPU Time (10 Million Integers)')
    ax2.set_ylabel('Time (ms)')
    
    # zoom in on the y-axis so the time difference is visually obvious
    min_time = df['CPU_Time_ms'].min()
    max_time = df['CPU_Time_ms'].max()
    ax2.set_ylim(min_time - 150, max_time + 150) # add a small buffer
    
    # add exact millisecond text
    ax2.bar_label(bars2, labels=[f'{x} ms' for x in df['CPU_Time_ms']], padding=3, fontweight='bold', fontsize=12)
    
    plt.tight_layout()
    plt.savefig('graph_10m_showdown.png')
    plt.show()
    print("Saved: graph_10m_showdown.png")

# Main
if __name__ == "__main__":
    print("Starting Data Analysis...")

    try:
        # replace file directory according to your computer
        # run Task c.i
        plot_size_vs_comparisons('data/experiment1_fixed_s.csv')
    except FileNotFoundError:
        print("Waiting on 'experiment1_fixed_s.csv'...")

    try:
        # run Task c.ii
        plot_s_vs_comparisons('data/experiment2_fixed_n.csv')
    except FileNotFoundError:
        print("Waiting on 'experiment2_fixed_n.csv'...")

    try:
        # run Task d
        plot_showdown('data/experiment3.csv')
    except FileNotFoundError:
        print("Waiting on 'experiment3.csv')")