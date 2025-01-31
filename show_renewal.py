import matplotlib.pyplot as plt
import pandas as pd

# CSVファイルを読み込む
df = pd.read_csv('walk_data.csv', header=None, names=['value'])

# インデックスを時間として使用
time = df.index

# 図とaxesオブジェクトを作成
fig, ax = plt.subplots(figsize=(8, 8))

# 散布図をプロット
ax.scatter(time, df['value'], alpha=0.5, s=1)

# グラフの設定
ax.set_xlabel('time', fontsize=25)
ax.set_ylabel('count', fontsize=25)

# 目盛りの設定
ax.tick_params(direction='in', which='both', top=True, right=True, labelsize=20)
ax.tick_params(which='minor', top=True, right=True)  # 副目盛りを追加

# グリッド線を削除（デフォルトで非表示）

# グラフを保存
plt.savefig('walk_data_scatter.png', dpi=300, bbox_inches='tight')

print('散布図を walk_data_scatter.png として保存しました。')
