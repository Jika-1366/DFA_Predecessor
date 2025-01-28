import numpy as np
import matplotlib.pyplot as plt
import sys
import os

# スクリプトの場所を取得
script_dir = os.path.dirname(os.path.abspath(__file__))
# プロジェクトルートを取得（2階層上）
project_root = os.path.dirname(os.path.dirname(script_dir))

# 相対パスとルートパスの両方を追加
sys.path.append(os.path.join(script_dir, "../../"))  # 相対パス
sys.path.append(project_root)  # プロジェクトルート

from formulas.calculate_intercept_0129 import calculate_intercept

# alphaの範囲を設定
alpha_range = np.arange(1.0, 2.01, 0.01)
coefs = []

# 各alphaに対してcoefを計算
for alpha in alpha_range:
    log_and_sqrt_coef = calculate_intercept(alpha=alpha)
    if log_and_sqrt_coef is not None:
        coef = np.exp(log_and_sqrt_coef)**2
        coefs.append(coef)
    else:
        coefs.append(np.nan)

# グラフの作成
plt.figure(figsize=(10, 6))
plt.plot(alpha_range, coefs)
plt.xlabel('α')
plt.ylabel('coefficient')
plt.title('Coefficient vs α')
plt.grid(True)

# グラフを保存
plt.savefig('projects/split_superposition/graphs/coefficient_vs_alpha.png')
plt.close()

# 結果を表示
for alpha, coef in zip(alpha_range, coefs):
    if not np.isnan(coef):
        print(f"α = {alpha:.2f}: coefficient = {coef:.6e}")
