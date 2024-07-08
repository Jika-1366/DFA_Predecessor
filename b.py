"""
このコードは、べき乗分布の指数αと、そこから生成されたランダムウォークの時系列データに対して
計算された構造関数の指数βの関係をプロットする。
"""

import matplotlib.pyplot as plt

# データを準備
alpha_values = [0.1, 0.3, 0.5, 0.7, 0.9, 1.1, 1.3, 1.5, 1.7, 2.1, 2.3, 2.7]
slope_values = [0.50, 0.51, 0.50, 0.50, 0.51, 0.49, 0.58, 0.49, 0.57, 0.51, 0.50, 0.50]

# グラフを作成
plt.plot(alpha_values, slope_values, marker='o')

# グラフのタイトルと軸ラベルを設定
plt.title('Slope vs. Alpha')
plt.xlabel('Alpha')
plt.ylabel('Slope')

# グラフを表示
plt.show()