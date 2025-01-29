import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# モデル関数を定義
def model_function(x, a, alpha, b, beta):
    return a * x**alpha + b * x**beta

# サンプルデータを生成
x_data = np.linspace(1, 10, 100)
y_data = 2 * x_data**1.5 + 3 * x_data**0.5 + np.random.normal(0, 0.5, 100)

# 初期値を設定
initial_guess = [0.5, 1.5, 0.5, 1.5]

# パラメータの範囲を設定
# bounds=([a_min, alpha_min, b_min, beta_min], [a_max, alpha_max, b_max, beta_max])
bounds = ([0, 1, 0, 1], [1, 2, 1, 2])

# フィッティング
params, covariance = curve_fit(model_function, x_data, y_data, p0=initial_guess, bounds=bounds)

# 結果を表示
a, alpha, b, beta = params
print(f"a: {a:.4f}, alpha: {alpha:.4f}, b: {b:.4f}, beta: {beta:.4f}")

# フィッティング結果をプロット
y_fitted = model_function(x_data, *params)
plt.scatter(x_data, y_data, label='Data')
plt.plot(x_data, y_fitted, label='Fitted curve', color='red')
plt.legend()
plt.show()