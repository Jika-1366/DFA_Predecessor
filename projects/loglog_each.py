import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob
import os
import re



def calcualte_F_calc_alpha_above2(l, alpha):
    global coffi_alpha_above2
    mu = alpha / (alpha - 1)
    
    c1 = (1+2*alpha-alpha**2)/(2*alpha*(alpha-2))

    coffi_alpha_above2 =  (1+2*c1 )/ (15 * mu)
    #coffi_alpha_above2 =  (1+c1 )/ (15 * mu)
    #coffi_alpha_above2 = 1/(15*mu)
    F2 =  (
        coffi_alpha_above2*l
    )
    F = np.sqrt(F2)
    return F



def extract_alpha(filename):
    match = re.search(r'(\d+(?:\.\d+)?)', os.path.basename(filename))
    if match:
        return float(match.group(1))
    return float('inf')

output_dir = "loglog_each"
os.makedirs(output_dir, exist_ok=True)

input_dir = "F2"
print(f"現在の読み込みデ\ィレクトリ: {input_dir}")

csv_files = sorted(glob.glob(f"{input_dir}/*.csv"), key=extract_alpha)

if not csv_files:
    print("CSVファイルが見つかりません。")
    exit()

global current_alpha, fig, ax
current_alpha = None
fig, ax = None, None

colors = ['blue', 'cyan', 'magenta', 'yellow', 'black', 'purple', 'orange']
markers = ['o', 's', '^', 'v', '<', '>', 'D']


def main():
    global current_alpha, fig, ax
    max_F_value = None
    
    for i, csv_file in enumerate(csv_files):
        alpha = extract_alpha(csv_file)
        
        if alpha != current_alpha:
            if fig is not None:
                plt.savefig(f"{output_dir}/graph_alpha_{current_alpha:.1f}.png")
                plt.close(fig)
            
            fig, ax = plt.subplots(figsize=(10, 6))
            current_alpha = alpha
            color_index = 0
        
        df = pd.read_csv(csv_file, header=None)
        l = df.iloc[:, 0].values
        F = df.iloc[:, 1].values
        
        print(l.tolist()[0])
        mask = (l > 0) & (F > 0)
        l = l[mask]
        F = F[mask]
        
        if len(l) == 0 or len(F) == 0:
            print(f"警告: {csv_file} には有効なデータがありません。スキップします。")
            continue
            
        # 最初のイテレーションでmax_F_valueを設定
        if i == 0:
            max_F_value = max(F)
        
        ax.loglog(l, F, marker=markers[color_index % len(markers)], linestyle='', 
                  label=f'Data_{os.path.basename(csv_file)}', color=colors[color_index % len(colors)])
        
        log_l = np.log(l)
        log_F = np.log(F)
        coefficients = np.polyfit(log_l, log_F, 1)
        slope = coefficients[0]
        intercept = coefficients[1]
        print(f"{csv_file}: slope={slope:.4f}, intercept={intercept:.4f}")
        fit_line = np.exp(intercept) * l ** slope
        #ax.loglog(l, fit_line, linestyle='--', color='blue', 
                  #label=f'Fit: F = {np.exp(intercept):.4f} * l^{slope:.4f}')
            
        global main_coffi, second_coffi, coffi_alpha_above2
        
        color_index += 1

        ax.set_title(f"Log-log graph (α = {alpha:.1f})", fontsize=18)
        ax.set_xlabel("l", fontsize=16)
        ax.set_ylabel("F", fontsize=16)
        ax.legend(fontsize=10)
        ax.set_xlim([min(l), max(l)])
        ax.grid(True)
        
        # 縦軸の範囲を固定
        ax.set_ylim([0.1, max_F_value])

        if alpha == 1.2:
            l_100 = 10**6
            F_100 = np.exp(intercept) * l_100 ** slope
            

    if fig is not None:
        plt.savefig(f"{output_dir}/graph_alpha_{current_alpha:.1f}.png")
        plt.close(fig)

    print("全てのグラフが生成されました。")

if __name__ == "__main__":
    main()