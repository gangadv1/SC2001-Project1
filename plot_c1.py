import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Load Part (c)(i) CSV data
df = pd.read_csv("c1_results.csv")

plt.figure(figsize=(9, 5))

# 1. Empirical Key Comparisons
plt.plot(df['n'], df['Key Comparisons'], 'bo-', label='Empirical Key Comparisons')

# 2. Theoretical curve: c * n * log2(n) scaled to match the empirical curve
c = df['Key Comparisons'].iloc[-1] / (df['n'].iloc[-1] * np.log2(df['n'].iloc[-1]))
theoretical = c * df['n'] * np.log2(df['n'])
plt.plot(df['n'], theoretical, 'r--', label=r'Theoretical $O(n \log_2 n)$')

# Formatting
plt.title('Part (c)(i): Key Comparisons vs Input Size $n$ (Fixed $S = 32$)', fontsize=12)
plt.xlabel('Input Size ($n$)', fontsize=11)
plt.ylabel('Key Comparisons', fontsize=11)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.tight_layout()

# Save image file
plt.savefig('graph_c1.png', dpi=300)
plt.show()
