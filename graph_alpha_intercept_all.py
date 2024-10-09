import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats
from formulas.a_and_b import calculate_a_and_b, calculate_pc, calculate_qc, calculate_c
from formulas.covariance1 import calculate_cova1_coffi1, calculate_cova1_inte1
from formulas.covariance2 import calculate_cova2_inte
from formulas.covariance3 import calculate_cova3_inte
from graph_alpha_intercept_all_above2 import theoretical_value_above2 as theoretical_value_high_alpha
# グラフを出力するファイル名を変数として定義
output_graph_file = 'graph_alpha_intercept_combined.png'

# ファイル名を表示
print(f'グラフは {output_graph_file} として保存されます。')

# 計算値を計算する関数 (1 < alpha < 2)
def theoretical_value_low_alpha(alpha):
    tau_0 = 1.0
    c = calculate_c(alpha=alpha, tau_0=tau_0)
    mu = (alpha * tau_0) / (alpha - 1)
    pc = calculate_pc(tau_0=tau_0, alpha=alpha, mu=mu)
    qc = calculate_qc(tau_0=tau_0, alpha=alpha, mu=mu)

    a_and_b = calculate_a_and_b(c, tau_0, alpha, mu)
    sum_coffi1 = calculate_cova1_coffi1(qc=qc, pc=pc, mu=mu) * calculate_cova1_inte1(alpha)
    coffi1 = sum_coffi1

    cova2_inte_list = calculate_cova2_inte(alpha=alpha)
    cova2_coffi_list = [a_and_b["b1"], a_and_b["b2"], a_and_b["b3"], a_and_b["b5"], a_and_b["b6"]]
    if len(cova2_inte_list) != len(cova2_coffi_list):
        raise ValueError("cova2_inte_list and cova2_coffi_list must have the same length")
    cova2_list = [cova2_inte_list[i]*cova2_coffi_list[i] for i in range(len(cova2_inte_list))]
    sum_coffi2 = 2.0*sum(cova2_list)
    coffi2 = -12*sum_coffi2

    cova3_inte_list = calculate_cova3_inte(alpha=alpha)
    cova3_coffi_list = cova2_coffi_list
    if len(cova3_inte_list) != len(cova3_coffi_list):
        raise ValueError("cova3_inte_list and cova3_coffi_list must have the same length")
    cova3_list = [cova3_inte_list[i]*cova3_coffi_list[i] for i in range(len(cova3_inte_list))]
    sum_coffi3 = 2.0*sum(cova3_list)
    coffi3 = -sum_coffi3

    coffi = coffi1+coffi2+coffi3
    sqrt_coffi = np.sqrt(coffi)
    return np.log(sqrt_coffi)


def prepare_intercept_data():
            # CSVファイルを読み込む
        data = pd.read_csv('alpha_all_intercepts.csv', header=None)

        # 列名を設定
        data.columns = ['alpha'] + [f'intercepts_{i}' for i in range(1, len(data.columns))]

        # alpha > 1 のデータのみを使用
        data = data[data['alpha'] > 1]
        # 平均とエラーを計算する関数
        def calculate_mean_and_error(row):
            intercepts = row.iloc[1:].dropna().astype(float)
            mean = intercepts.mean()
            error = intercepts.std() / np.sqrt(len(intercepts))
            return pd.Series({'mean_intercept': mean, 'error': error})

        # 平均とエラーを計算
        results = data.apply(calculate_mean_and_error, axis=1)
        data = data.join(results)
        return data

def main():
    try:
        data = prepare_intercept_data()
        # 計算値を追加
        def calculate_theoretical(alpha):
            if alpha < 1:
                return 0.0  # alpha < 1 の場合は計算しない
            elif 1 < alpha < 2:
                return theoretical_value_low_alpha(alpha)
            elif alpha > 2:
                return theoretical_value_high_alpha(alpha)
            else:
                return 0.0  # alpha が 1 または 2 の場合は計算しない

        data['theoretical'] = data['alpha'].apply(calculate_theoretical)

        # グラフを作成
        plt.figure(figsize=(12, 8))

        # 全データをプロット（エラーバー付き）
        plt.errorbar(data['alpha'], data['mean_intercept'], yerr=data['error'], fmt='o', color='blue', label='Intercept', capsize=5, markersize=8)

        # 計算値をプロット（エラーバーなし）
        plt.plot(data['alpha'], data['theoretical'], color='red', label='Theoretical', linewidth=2)

        plt.xlabel('Alpha', fontsize=30)
        plt.ylabel('Mean Intercept', fontsize=30)
        plt.title('Alpha vs Mean Intercept (Combined)', fontsize=34)
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
        print(f'エラーが発生しました: {str(e)}')  # エラー内容を表示
        print('エラーが発生した行:')  # エラーが発生した行を示すメッセージ
        import traceback  # トレースバックをインポート
        traceback.print_exc()  # エラーのトレースバックを表示

if __name__ == "__main__":
    main()