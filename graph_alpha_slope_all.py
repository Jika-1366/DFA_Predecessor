import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

# CSVファイルを読み込む
data = pd.read_csv('alpha_all_slopes.csv', header=None)

# 列名を設定
data.columns = ['alpha'] + [f'slope_{i}' for i in range(1, len(data.columns))]

# 平均とエラーを計算する関数
def calculate_mean_and_error(row):
    slopes = row.iloc[1:].dropna().astype(float)
    mean = slopes.mean()
    error = slopes.std() / np.sqrt(len(slopes))
    return pd.Series({'mean_slope': mean, 'error': error})

# 平均とエラーを計算
results = data.apply(calculate_mean_and_error, axis=1)
data = data.join(results)

# 1.3 <= alpha <= 1.8 の範囲のデータを抽出
mask = (data['alpha'] >= 1.3) & (data['alpha'] <= 1.8)
filtered_data = data[mask]

# 線形回帰を行う
slope, intercept, r_value, p_value, std_err = stats.linregress(filtered_data['alpha'], filtered_data['mean_slope'])

# グラフを作成
plt.figure(figsize=(12, 8))

# 全データをプロット（エラーバー付き）
plt.errorbar(data['alpha'], data['mean_slope'], yerr=data['error'], fmt='o', color='blue', label='m', capsize=5, markersize=8)

# フィッティング線を描画（横軸の範囲を拡張）
x_fit = np.linspace(1.1, 2.0, 100)
y_fit = slope * x_fit + intercept
plt.plot(x_fit, y_fit, color='red', label=f'y = {slope:.2f}x + {intercept:.2f}', linewidth=2)

# alpha > 2 の範囲で y = 0.5 の直線を描画
x_line = np.linspace(2, data['alpha'].max(), 100)
y_line = np.full_like(x_line, 0.5)
plt.plot(x_line, y_line, color='green', linestyle='--', label='y = 0.5', linewidth=2)

plt.xlabel('Alpha', fontsize=20)
plt.ylabel('Mean Slope', fontsize=20)
plt.title('Alpha vs Mean Slope', fontsize=24)
plt.legend(fontsize=16)
plt.grid(True)

# グラフの軸範囲を設定
plt.xlim(0.0, plt.xlim()[1])  
plt.ylim(0.4, 1.1)  # y軸の最大値を1.1に設定

# 軸のティックラベルのフォントサイズを大きくする
plt.xticks(fontsize=16)
plt.yticks(fontsize=16)

# グラフを保存
plt.savefig('graph_alpha_slope_all.png', dpi=300, bbox_inches='tight')
plt.close()

print('グラフが graph_alpha_slope_all.png として保存されました。')

print(slope)