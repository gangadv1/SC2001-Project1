import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Load CSV output from Part (c)(ii) / (c)(iii)
df = pd.read_csv("c2_results.csv")

n = 1_000_000 # Fixed input size

# Compute the theoretical comparison curve from lecture / reference
# Formula: n * log2(n/S) + n * S / 4
S_vals = df['S'].values
theoretical_comps = n * np.log2(n / S_vals) + (n * S_vals / 4.0)

# Create 1x2 subplot layout
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

# ----------------------------------------------------
# Left Graph: (c)(ii) Key Comparisons vs Threshold S
# ----------------------------------------------------
ax1.plot(df['S'], df['Key Comparisons'], marker='o', color='#1f77b4', label='Hybrid Merge Sort')
ax1.plot(df['S'], theoretical_comps, marker='x', linestyle='--', color='#ff7f0e', label=r'Theoretical $C(n,S) = n\log_2(n/S) + nS/4$')

ax1.set_title('(c)(ii): Key Comparisons vs Threshold S', fontsize=12)
ax1.set_xlabel('Threshold S', fontsize=11)
ax1.set_ylabel('Key Comparisons', fontsize=11)
ax1.grid(True, linestyle='-', alpha=0.6)
ax1.legend(loc='upper left', framealpha=0.9)
ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0)) # Uses 1e7 notation on top

# ----------------------------------------------------
# Right Graph: (c)(ii) CPU Time vs Threshold S
# ----------------------------------------------------
ax2.plot(df['S'], df['CPU Time (ms)'], marker='s', color='#1f77b4', label='Hybrid Merge Sort')

ax2.set_title('(c)(ii): CPU Time vs Threshold S', fontsize=12)
ax2.set_xlabel('Threshold S', fontsize=11)
ax2.set_ylabel('CPU Time (ms)', fontsize=11)
ax2.grid(True, linestyle='-', alpha=0.6)
ax2.legend(loc='upper left', framealpha=0.9)

plt.tight_layout()
plt.savefig('c2_comparison_and_time.png', dpi=300)
plt.show()
