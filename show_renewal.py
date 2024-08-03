import matplotlib.pyplot as plt
import pandas as pd

# CSVファイルを読み込む
df = pd.read_csv('walk_data.csv', header=None, names=['value'])

# インデックスを時間として使用
time = df.index

# 散布図をプロット
plt.figure(figsize=(6, 6))  # 横幅を狭くし、縦長の図にする
plt.scatter(time, df['value'], alpha=0.5, s=1)

# グラフの設定
plt.title('renewal_walk', fontsize=19)  # タイトルのフォントサイズを大きくする
plt.xlabel('time', fontsize=25)  # x軸ラベルのフォントサイズを大きくする
plt.ylabel('count', fontsize=25)  # y軸ラベルのフォントサイズを大きくする

# グリッドを表示
plt.grid(True, linestyle='--', alpha=0.7)

# グラフを保存
plt.savefig('walk_data_scatter.png', dpi=300, bbox_inches='tight')

print('散布図を walk_data_scatter.png として保存しました。')
