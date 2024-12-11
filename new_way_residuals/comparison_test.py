import numpy as np
import matplotlib.pyplot as plt

def get_residuals_legacy(y):
    n = len(y)
    x = np.arange(1, n+1)
    
    x_mean = np.mean(x)
    y_mean = np.mean(y)
    
    numerator = np.sum((x - x_mean) * (y - y_mean))
    denominator = np.sum((x - x_mean) ** 2)
    
    slope = numerator / denominator
    intercept = y_mean - slope * x_mean
    
    y_pred = slope * x + intercept
    residuals = y - y_pred
    return np.sum(residuals ** 2)/n

def get_residuals_direct(y):
    n = len(y)
    mean = np.mean(y)
    
    sq_sum = np.sum(y * y)
    weighted_sum = np.sum([(i+1) * val for i, val in enumerate(y)])
    
    v2_y = (sq_sum / n) - mean * mean
    second_term = (12.0 * (n+1)/(n-1)) * ((weighted_sum/n/(n+1) - mean/2) ** 2)
    
    return v2_y - second_term

# テストデータの生成
np.random.seed(42)
n_points = 100
x = np.linspace(0, 10, n_points)
y = 2 * x + 1 + np.random.normal(0, 1, n_points)

# 両方の手法で残差を計算
legacy_residuals = get_residuals_legacy(y)
direct_residuals = get_residuals_direct(y)

print(f"Legacy手法による残差: {legacy_residuals}")
print(f"Direct手法による残差: {direct_residuals}")
print(f"差分: {abs(legacy_residuals - direct_residuals)}")
print(f"相対差分: {abs(legacy_residuals - direct_residuals) / legacy_residuals * 100}%")

# 結果の可視化
plt.figure(figsize=(12, 6))
plt.subplot(1, 2, 1)
plt.scatter(x, y, label='データ点')
plt.title('データ分布')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()

plt.subplot(1, 2, 2)
plt.bar(['Legacy', 'Direct'], [legacy_residuals, direct_residuals])
plt.title('残差の比較')
plt.ylabel('残差の値')

plt.tight_layout()
plt.savefig('residuals_comparison.png')
plt.close()
