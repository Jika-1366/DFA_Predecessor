from formulas.a_and_b import calculate_a_and_b, calculate_pc, calculate_qc, calculate_c

from formulas.covariance1 import calculate_cova1_coffi1, calculate_cova1_inte1
from formulas.covariance2 import calculate_cova2_inte
from formulas.covariance3 import calculate_cova3_inte

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats
from formulas.calculate_intercept import calculate_intercept

# グラフを出力するファイル名を変数として定義
output_graph_file = 'graph_alpha_intercept_between1and2.png'

# ファイル名を表示
print(f'グラフは {output_graph_file} として保存されます。')


def main():
    try:
        # CSVファイルを読み込む
        data = pd.read_csv('alpha_all_intercepts.csv', header=None)

        # 1 < alpha < 2.1 のデータのみを選択
        data = data[(data.iloc[:, 0] > 1) & (data.iloc[:, 0] < 2.1)]

        # 列名を設定
        data.columns = ['alpha'] + [f'intercepts_{i}' for i in range(1, len(data.columns))]

        # 平均とエラーを計算する関数
        def calculate_mean_and_error(row):
            intercepts = row.iloc[1:].dropna().astype(float)
            mean = intercepts.mean()
            error = intercepts.std() / np.sqrt(len(intercepts))
            return pd.Series({'mean_intercept': mean, 'error': error})

        # 平均とエラーを計算
        results = data.apply(calculate_mean_and_error, axis=1)
        data = data.join(results)

        # 計算値
        data['theoretical'] = data['alpha'].apply(lambda alpha: calculate_intercept(alpha))

        # 直線の式を計算
        slope, intercept, r_value, p_value, std_err = stats.linregress(data['alpha'], data['mean_intercept'])
        print(f'直線の式: y = {slope:.4f}x + {intercept:.4f}')

        # グラフを作成
        plt.figure(figsize=(12, 8))

        # 全データをプロット（エラーバー付き）
        plt.errorbar(data['alpha'], data['mean_intercept'], yerr=data['error'], fmt='o', color='blue', label='Intercept', capsize=5, markersize=8)

        # 計算値をプロット（エラーバーなし）
        plt.plot(data['alpha'], data['theoretical'], color='red', label='Theoretical', linewidth=2)

        # 直線をプロット
        #plt.plot(data['alpha'], slope * data['alpha'] + intercept, color='green', linestyle='--', label=f'Fit: y = {slope:.4f}x + {intercept:.4f}')

        plt.xlabel('Alpha', fontsize=30)
        plt.ylabel('Mean Intercept', fontsize=30)
        plt.title('Alpha vs Mean Intercept', fontsize=34)
        plt.legend(fontsize=26)
        plt.grid(True)

        # グラフの軸範囲を自動調整
        plt.autoscale()

        # 軸のティックラベルのフォントサイズを大きくする
        plt.xticks(fontsize=16)
        plt.yticks(fontsize=16)

        # グラフを保存
        plt.savefig(output_graph_file, dpi=300, bbox_inches='tight')
        plt.close()

        print(f'グラフが {output_graph_file} として保存されました。')
    except Exception as e:
        import traceback
        traceback_str = traceback.format_exc()
        print(f"エラーが発生しました: {e}")
        print(f"トレースバック情報:\n{traceback_str}")

if __name__ == "__main__":
    main()