import numpy as np
import csv

# べき乗分布のパラメータを設定
tau_0 = 1.0

# alphaの値を0.1から3まで0.2ずつ増やしながらループ
for alpha in np.arange(0.1, 3.1, 0.2):
    # べき乗分布からたくさんのべき関数による値、numpy配列を作成。
    #np.random.uniform(0, 1, sample_amount)がまさに0~1の一様分布のxです。
    sample_amount = 10**5
    samples = (tau_0 / np.random.uniform(0, 1, sample_amount)) ** (1 / (alpha - 1))
    samples = (tau_0*(np.random.uniform(0, 1, sample_amount)**(-1/alpha)))


    # ランダムウォークを生成
    walk = np.cumsum(samples)

    # ランダムウォークをCSVファイルに保存
    filename = f"beki_walk_alpha_{alpha:.1f}.csv"
    with open(filename, 'w', newline='') as file:
        writer = csv.writer(file)
        for position in walk:
            writer.writerow([position])
