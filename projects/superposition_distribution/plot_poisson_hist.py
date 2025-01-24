import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# データの読み込み
intervals = np.loadtxt('superposed_intervals_100.csv')

# 指数分布の関数定義（1パラメータ）
def exp_func(x, lambda_param):
    return lambda_param * np.exp(-lambda_param * x)

# べき分布の関数定義（tau_0のみをパラメータとして使用）
def power_law_func(x, tau_0):
    return (1.5/tau_0) * ((x/tau_0)**(-2.5))

# ヒストグラムのデータを取得
bins = np.arange(0, 1.01, 0.0001)
hist, bin_edges = np.histogram(intervals, bins=bins, density=True)
bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2

# 指数分布のフィッティング
popt_exp, _ = curve_fit(exp_func, bin_centers, hist, p0=[1])
lambda_fit = popt_exp[0]

# べき分布のフィッティング
# 0に近い値を除外してフィッティング（対数が発散するため）
mask = bin_centers > 0.01
popt_power, _ = curve_fit(power_law_func, bin_centers[mask], hist[mask], p0=[1])
tau_0_fit = popt_power[0]

# フィッティング結果の曲線用のx値
x_fit = np.linspace(0.01, 1.0, 1000)  # べき分布のため0は除外
y_fit_exp = exp_func(x_fit, lambda_fit)
y_fit_power = power_law_func(x_fit, tau_0_fit)

# グラフ1: ヒストグラムのみ
plt.figure(figsize=(12, 8))
plt.hist(intervals, bins=bins, density=True, alpha=0.7, label='Empirical distribution')
plt.xlim(0, 1.0)
plt.xlabel('Time Interval')
plt.ylabel('Density')
plt.title('Interval Distribution (Superposition of 100 Power Laws with α = 1.5)')
plt.grid(True, alpha=0.3)
plt.legend()
plt.savefig('histogram_superposed_100_no_fit.png', dpi=300, bbox_inches='tight')
plt.close()

# グラフ2: ヒストグラム + フィッティング
plt.figure(figsize=(12, 8))
plt.hist(intervals, bins=bins, density=True, alpha=0.7, label='Empirical distribution')
plt.plot(x_fit, y_fit_exp, 'r-', label=f'Exponential fit: λ={lambda_fit:.2f}', alpha=0.8)
#plt.plot(x_fit, y_fit_power, 'g-', label=f'Power law fit: τ₀={tau_0_fit:.4f}', alpha=0.8)
plt.xlim(0, 1.0)
plt.xlabel('Time Interval')
plt.ylabel('Density')
plt.title('Interval Distribution with Fits\n(Superposition of 100 Power Laws with α = 1.5)')
plt.grid(True, alpha=0.3)
plt.legend()
plt.savefig('histogram_superposed_100_with_fit.png', dpi=300, bbox_inches='tight')
plt.close()

# 基本統計量とフィッティングパラメータの出力
print(f"Number of intervals: {len(intervals)}")
print(f"Mean interval: {np.mean(intervals):.4f}")
print(f"Median interval: {np.median(intervals):.4f}")
print(f"Std deviation: {np.std(intervals):.4f}")
print(f"\nFitting parameters:")
print(f"Exponential: λ = {lambda_fit:.4f}")
print(f"Power law: τ₀ = {tau_0_fit:.4f}")
print(f"Fitted functions:")
print(f"Exponential: f(x) = {lambda_fit:.4f}·exp(-{lambda_fit:.4f}x)")
print(f"Power law: f(x) = {1.5/tau_0_fit:.4f}·(x/{tau_0_fit:.4f})^(-2.5)") 