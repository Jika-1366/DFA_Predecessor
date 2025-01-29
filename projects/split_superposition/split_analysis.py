import numpy as np
import matplotlib.pyplot as plt
import sys
import os
import glob
import pandas as pd
from enum import Enum, auto

# スクリプトの場所を取得
script_dir = os.path.dirname(os.path.abspath(__file__))
# プロジェクトルートを取得（2階層上）
project_root = os.path.dirname(os.path.dirname(script_dir))

# 相対パスとルートパスの両方を追加
sys.path.append(os.path.join(script_dir, "../../"))  # 相対パス
sys.path.append(project_root)  # プロジェクトルート

from fittings import model_function, fit_data, grid_search_fit

# フィッティング手法の選択
class FittingMethod(Enum):
    CURVE_FIT = auto()      # scipy.optimize.curve_fitを使用
    GRID_SEARCH = auto()    # グリッドサーチによる最小二乗法

# ここでフィッティング手法を選択
FITTING_METHOD = FittingMethod.CURVE_FIT  # または FittingMethod.GRID_SEARCH

def process_file(file_path):
    # CSVファイルを読み込む
    df = pd.read_csv(file_path)
    
    # 全データを使用
    x_data = df.iloc[:, 0].values  # x軸データ
    y_data = df.iloc[:, 1].values  # y軸データ

    try:
        # フィッティング手法の選択
        if FITTING_METHOD == FittingMethod.CURVE_FIT:
            result = fit_data(x_data, y_data)
            method_name = "Curve Fit"
        else:
            result = grid_search_fit(x_data, y_data)
            method_name = "Grid Search"
        
        alpha, beta = result.params
        
        # フィッティング結果をプロット
        plt.figure(figsize=(10, 6))
        y_fitted = model_function(x_data, alpha, beta)
        plt.scatter(x_data, y_data, label='Data', alpha=0.6)
        plt.plot(x_data, y_fitted, label='Fitted curve', color='red')
        
        # グラフの設定
        plt.xlabel('x')
        plt.ylabel('F2')
        title = f'{method_name} Result (α={alpha:.4f}, β={beta:.4f}, R²={result.r_squared:.4f})'
        if result.errors is not None:
            alpha_err, beta_err = result.errors
            title += f'\nErrors: Δα={alpha_err:.4f}, Δβ={beta_err:.4f}'
        plt.title(title)
        plt.legend()
        plt.grid(True, which='both', linestyle='--', alpha=0.7)
        
        # 両軸を対数スケールに
        plt.xscale('log')
        plt.yscale('log')
        
        # グラフを保存
        base_name = os.path.basename(file_path)
        output_dir = os.path.join(script_dir, "graphs")
        
        # フィッティング手法に基づいてサブディレクトリを作成
        method_dir = "curve_fit" if FITTING_METHOD == FittingMethod.CURVE_FIT else "grid_search"
        output_dir = os.path.join(output_dir, method_dir)
        os.makedirs(output_dir, exist_ok=True)
        
        # ファイル名からフィッティング手法の接尾辞を削除（もはや不要）
        graph_file = os.path.join(output_dir, f"fit_result_{os.path.splitext(base_name)[0]}.png")
        plt.savefig(graph_file, dpi=300, bbox_inches='tight')
        plt.close()
            
        print(f"Processed {base_name} using {method_name}:")
        print(f"  α={alpha:.4f}, β={beta:.4f}, R²={result.r_squared:.4f}")
        if result.errors is not None:
            print(f"  Errors: Δα={alpha_err:.4f}, Δβ={beta_err:.4f}")
        return True
    except Exception as e:
        print(f"Error processing {file_path}: {str(e)}")
        return False

def main():
    # avg_F2ディレクトリ内のすべてのCSVファイルを処理
    data_dir = os.path.join(script_dir, "avg_F2")
    csv_files = glob.glob(os.path.join(data_dir, "*.csv"))
    
    for file_path in csv_files:
        if os.path.basename(file_path) != ".gitkeep":
            process_file(file_path)

if __name__ == "__main__":
    main()