import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

def prepare_data_above2():
    # CSVファイルを読み込む
    data = pd.read_csv('alpha_all_intercepts.csv', header=None)

    #alpha>2のデータだけ用いる。
    # alpha > 2 のデータのみを選択
    data = data[data.iloc[:, 0] > 2]

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
    return data

# 計算値を計算する関数
def theoretical_value_hand(alpha):
    if alpha <= 1 or alpha <= 2:
        return np.nan
    mu = alpha / (alpha - 1)
    sigma2 = alpha / (((alpha - 2)**2) * (alpha - 1))
    coffi = ((-1 + (1/mu**3) + 15/2) * sigma2 - 1/mu) / 15
    sqrt_coffi = np.sqrt(coffi)
    return np.log(sqrt_coffi)

def theoretical_value_mathmatica2(alpha):
    mu = alpha / (alpha - 1)
    sigma2 = alpha / (((alpha - 2)**2) * (alpha - 1))
    coffi =  (
            1 / (15 * mu)
            - 13 * sigma2 / 30
            + 0.5 * sigma2
            - sigma2 / (15 * mu**3)
    )
    sqrt_coffi = np.sqrt(coffi)
    return np.log(sqrt_coffi)


def theoretical_value_mathmatica(alpha):
    mu = alpha / (alpha - 1)
    coffi = (1/(15*mu))
    sqrt_coffi = np.sqrt(coffi)
    return np.log(sqrt_coffi)


def main():
    data = prepare_data_above2()

    # 計算値
    data['theoretical'] = data['alpha'].apply(theoretical_value_mathmatica)
    # 計算値
    data['theoretical_hand'] = data['alpha'].apply(theoretical_value_hand)


    # 直線の式を計算
    #slope, intercept, r_value, p_value, std_err = stats.linregress(data['alpha'], data['mean_intercept'])
    #print(f'直線の式: y = {slope:.4f}x + {intercept:.4f}')

    # グラフを作成
    plt.figure(figsize=(12, 8))

    # 全データをプロット（エラーバー付き）
    plt.errorbar(data['alpha'], data['mean_intercept'], yerr=data['error'], fmt='o', color='blue', label='Intercept', capsize=5, markersize=8)

    # 計算値をプロット（エラーバーなし）
    plt.plot(data['alpha'], data['theoretical'], color='red', label='Theoretical', linewidth=2)

    # 計算値_手計算をプロット（エラーバーなし）
    plt.plot(data['alpha'], data['theoretical_hand'], color='purple', label='Theoretical_hand', linewidth=2)

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
    plt.savefig('graph_alpha_intercept_all.png', dpi=300, bbox_inches='tight')
    plt.close()

    print('グラフが graph_alpha_intercept_all.png として保存されました。')

if __name__ == "__main__":
    main()