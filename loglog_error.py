import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import matplotlib.font_manager as fm

# Load CSV file
data = pd.read_csv('alpha_not_l_all_F.csv', header=None)

# Get l and F values
l_values = data[0]
F_values = data[1]

# Calculate mean and standard error of F for each l
grouped = data.groupby(0)
mean_F = grouped[1].mean()
std_error_F = grouped[1].sem()  # Calculate standard error

# Create log-log plot
plt.figure(figsize=(10, 6))
plt.errorbar(mean_F.index, mean_F, yerr=std_error_F, fmt='o', ecolor='r', capsize=5, label='F mean with error bars')

# Set graph properties
plt.xscale('log')
plt.yscale('log')
plt.xlabel('l (log scale)')
plt.ylabel('F (log scale)')
plt.title('Log-Log Plot of F vs l (with error bars)')
plt.legend()

# Use a font that supports English characters
plt.rcParams['font.family'] = 'DejaVu Sans'

# Display the graph
plt.tight_layout()
plt.show()


# csvのような形式で、1列目にl, 2列目に平均のFをprint
for l, f in zip(mean_F.index, mean_F):
    print(f"{l},{f}")

