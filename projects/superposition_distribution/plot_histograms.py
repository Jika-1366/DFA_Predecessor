import os
import numpy as np
import matplotlib.pyplot as plt
from glob import glob

# 出力ディレクトリの作成
output_dir = "histograms_alphas"
os.makedirs(output_dir, exist_ok=True)

# CSVファイルの読み込みとヒストグラムの作成
def create_histogram(file_path, max_x, max_y):
    # データの読み込み
    intervals = np.loadtxt(file_path)
    
    # ヒストグラムの作成
    plt.figure(figsize=(10, 6))
    bins = np.arange(0, 50.01, 0.01)  # 0から50まで、0.01刻み
    plt.hist(intervals, bins=bins, density=True, alpha=0.7)
    
    # グラフの設定
    plt.xlim(0, max_x)
    plt.ylim(0, max_y)
    plt.xlabel('Time Interval')
    plt.ylabel('Density')
    
    # ファイル名からalpha値を抽出
    filename = os.path.basename(file_path)
    alpha_values = filename.split('alpha')[1].split('.csv')[0]
    plt.title(f'Interval Distribution (α = {alpha_values})')
    
    # グラフの保存
    output_path = os.path.join(output_dir, f'hist_{alpha_values}.png')
    plt.savefig(output_path, dpi=300, bbox_inches='tight')
    plt.close()

def main():
    # 最初のファイルからx軸とy軸の最大値を決定
    first_file = glob('intervals_alphas/intervals_alpha*.csv')[0]
    first_data = np.loadtxt(first_file)
    
    # ヒストグラムを作成して最大値を取得
    plt.figure()
    counts, bins, _ = plt.hist(first_data, bins=np.arange(0, 50.01, 0.01), density=True)
    max_y = max(counts) * 1.1  # 余白を10%追加
    max_x = 50  # 指定された最大値
    plt.close()
    
    # すべてのCSVファイルに対してヒストグラムを作成
    csv_files = glob('intervals_alphas/intervals_alpha*.csv')
    total_files = len(csv_files)
    
    for i, file_path in enumerate(csv_files, 1):
        print(f'Processing {i}/{total_files}: {os.path.basename(file_path)}')
        create_histogram(file_path, max_x, max_y)
    
    print(f'\nHistograms have been saved to {output_dir}/')

if __name__ == "__main__":
    main() 