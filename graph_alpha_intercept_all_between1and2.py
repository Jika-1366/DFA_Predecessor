from formulas.a_and_b import calculate_a_and_b, calculate_pc, calculate_qc, calculate_c

from formulas.covariance1 import calculate_cova1_coffi1, calculate_cova1_inte1
from formulas.covariance2 import calculate_cova2_inte
from formulas.covariance3 import calculate_cova3_inte

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

# グラフを出力するファイル名を変数として定義
output_graph_file = 'graph_alpha_intercept_between1and2.png'

# ファイル名を表示
print(f'グラフは {output_graph_file} として保存されます。')

#b_i * b_inte_iで、i=4は抜いて足し算すれば、F^2の第二項が得られます。

# 計算値を計算する関数
def theoretical_value(alpha):
    tau_0 = 1.0
    c = calculate_c(alpha=alpha, tau_0=tau_0)
    mu = (alpha * tau_0) / (alpha - 1)
    pc = calculate_pc(tau_0=tau_0, alpha=alpha, mu=mu)
    qc = calculate_qc(tau_0=tau_0, alpha=alpha, mu=mu)

    a_and_b= calculate_a_and_b(c, tau_0, alpha, mu)
    sum_coffi1 = calculate_cova1_coffi1(qc=qc, pc=pc, mu=mu )*calculate_cova1_inte1(alpha)
    coffi1 = sum_coffi1

    cova2_inte_list= calculate_cova2_inte(alpha=alpha)
    
    cova2_coffi_list = [a_and_b["b1"], a_and_b["b2"], a_and_b["b3"], a_and_b["b5"], a_and_b["b6"]]
    if len(cova2_inte_list) != len(cova2_coffi_list):
        raise ValueError("cova2_inte_list and cova2_coffi_list must have the same length")
    cova2_list = [cova2_inte_list[i]*cova2_coffi_list[i] for i in range(len(cova2_inte_list))]
    # cova2_inte_listの要素をそれぞれ2倍する。なぜならまだ重積分の一か所だったから
    sum_coffi2 = 2.0*sum(cova2_list)
    coffi2= -12*sum_coffi2

    cova3_inte_list = calculate_cova3_inte(alpha=alpha)
    cova3_coffi_list = cova2_coffi_list
    if len(cova3_inte_list) != len(cova3_coffi_list):
        raise ValueError("cova3_inte_list and cova3_coffi_list must have the same length")
    cova3_list = [cova3_inte_list[i]*cova3_coffi_list[i] for i in range(len(cova3_inte_list))]
    # cova3_inte_listの要素をそれぞれ2倍する。なぜならまだ重積分の一か所だったから
    sum_coffi3 = 2.0*sum(cova3_list)
    coffi3 = -sum_coffi3

    coffi = coffi1 + coffi2 + coffi3
    print(f"coffi: {coffi:.4f}")

    sqrt_coffi = np.sqrt(coffi)
    return np.log(sqrt_coffi)

def main():
    try:
        # CSVファイルを読み込む
        data = pd.read_csv('alpha_all_intercepts.csv', header=None)

        # 1 < alpha < 2 のデータのみを選択
        data = data[(data.iloc[:, 0] > 1) & (data.iloc[:, 0] < 2)]

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
        data['theoretical'] = data['alpha'].apply(lambda alpha: theoretical_value(alpha))

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
        print(f'エラーが発生しました: {str(e)}')

if __name__ == "__main__":
    main()