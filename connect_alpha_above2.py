import numpy as np
import matplotlib.pyplot as plt

def calculate_coefficients(alpha):
    mu = alpha / (alpha - 1)
    c1 = (1 + 2*alpha - alpha**2) / (2*alpha * (alpha-2))
    coffi_alpha_above2_former = (1 + 2*c1) / (15 * mu)
    coffi_alpha_above2_later = (1 + c1) / (15 * mu)
    coffi_alpha_above2_another = 1 / (15 * mu)
    
    return coffi_alpha_above2_former, coffi_alpha_above2_later, coffi_alpha_above2_another

alpha_range = np.linspace(2.0, 3.5, 100)
coffi_former = []
coffi_later = []
coffi_another = []

for alpha in alpha_range:
    former, later, another = calculate_coefficients(alpha)
    coffi_former.append(former)
    coffi_later.append(later)
    coffi_another.append(another)

plt.figure(figsize=(10, 6))
plt.plot(alpha_range, coffi_former, label='Former coefficient', linewidth=2, color='red')
plt.plot(alpha_range, coffi_later, label='Later coefficient', linewidth=2, color='green')
plt.ylim(0 ,0.6)
plt.xlabel('Alpha')
plt.ylabel('Coefficient')
plt.title('Coefficients vs Alpha (2.0 to 3.5)')
plt.legend()
plt.grid(True)
plt.show()

# 数値解析でcoffi_alpha_above2_formerとlaterが同じになる位置を求める
alpha_range_fine = np.linspace(2.0, 3.5, 10000)
min_diff = float('inf')
min_diff_alpha = None

for alpha in alpha_range_fine:
    former, later, _ = calculate_coefficients(alpha)
    diff = abs(former - later)
    if diff < min_diff:
        min_diff = diff
        min_diff_alpha = alpha

print(f"coffi_alpha_above2_formerとlaterが最も近くなるalpha: {min_diff_alpha:.6f}")