import csv
import matplotlib.pyplot as plt
import os

def plot_csv_columns(filepath, columns_to_plot, save_dir, loglog=True):
    """
    CSVファイルの指定された列をプロットし、保存する。

    Args:
        filepath (str): CSVファイルのパス。
        columns_to_plot (list): プロットする列番号のリスト（0始まり）。
        save_dir (str): グラフを保存するディレクトリパス。
        loglog (bool): Trueの場合はloglogプロット、Falseの場合は通常プロット。
    """
    x_values = []
    y_values = [[] for _ in range(len(columns_to_plot))]

    with open(filepath, 'r') as csvfile:
        reader = csv.reader(csvfile)
        header = next(reader)  # ヘッダー行を読み飛ばす
        for row in reader:
            if row:  # 空行でないか確認
                try:
                    x_values.append(float(row[0]))  # 0列目をx軸の値として使用
                    for i, col_index in enumerate(columns_to_plot):
                        y_values[i].append(float(row[col_index]))
                except (ValueError, IndexError) as e:
                    print(f"Error processing row: {row} - {e}")
                    continue

    plt.figure(figsize=(10, 6))  # プロットのサイズ調整

    colors = ['blue', 'green', 'red', 'purple']  # プロットの色を設定
    markers = ['o', 's', '^', 'v']  # マーカーの種類を設定

    for i, col_index in enumerate(columns_to_plot):
        if loglog:
            plt.loglog(x_values, y_values[i], label=f'Column {col_index}',
                     color=colors[i % len(colors)], marker=markers[i % len(markers)],
                     linestyle='', markersize=4)
        else:
            plt.plot(x_values, y_values[i], label=f'Column {col_index}',
                    color=colors[i % len(colors)], marker=markers[i % len(markers)],
                    linestyle='', markersize=4)

    plt.xlabel('n' + (' (log scale)' if loglog else ''))
    plt.ylabel('F2' + (' (log scale)' if loglog else ''))
    plt.title(('Log-log' if loglog else 'Normal') + ' Plot of Specified Columns from CSV')
    plt.legend()
    plt.grid(True)
    
    # グラフ保存用のディレクトリを作成
    os.makedirs(save_dir, exist_ok=True)
    
    # ファイル名を生成
    plot_type = 'loglog' if loglog else 'normal'
    save_path = os.path.join(save_dir, f'plot_{plot_type}.png')
    
    plt.savefig(save_path)
    plt.close()
    print(f"グラフを保存しました: {save_path}")

# 使用例：
filepath = 'data_1_1_1_5.csv'
columns_to_plot = [1, 2, 4, 6]  # プロットしたい列番号 (0始まり)
save_dir = 'graphs'  # 保存先ディレクトリ

# loglogバージョンと通常バージョンの両方を生成
plot_csv_columns(filepath, columns_to_plot, save_dir, loglog=True)
plot_csv_columns(filepath, columns_to_plot, save_dir, loglog=False)