import numpy as np
import matplotlib.pyplot as plt
import sys
import os
import glob
import pandas as pd
import re
from enum import Enum, auto

# スクリプトの場所を取得
script_dir = os.path.dirname(os.path.abspath(__file__))
# プロジェクトルートを取得（2階層上）
project_root = os.path.dirname(os.path.dirname(script_dir))

# 相対パスとルートパスの両方を追加
sys.path.append(os.path.join(script_dir, "../../"))  # 相対パス
sys.path.append(project_root)  # プロジェクトルート

from fittings import model_function, fit_data, grid_search_fit
from formulas.calculate_intercept_0129 import calculate_intercept

# フィッティング手法の選択
class FittingMethod(Enum):
    CURVE_FIT = auto()      # scipy.optimize.curve_fitを使用
    GRID_SEARCH = auto()    # グリッドサーチによる最小二乗法

# ここでフィッティング手法を選択
FITTING_METHOD = FittingMethod.GRID_SEARCH  # または FittingMethod.GRID_SEARCH

def extract_alphas_from_filename(filename):
    # ファイル名からalphaの値を抽出
    pattern = r'alpha(\d+\.\d+)_(\d+\.\d+)'
    match = re.search(pattern, filename)
    if match:
        return float(match.group(1)), float(match.group(2))
    return None, None

def theoretical_curve(x, alpha):
    # 理論値の計算
    return x**alpha

def process_file(file_path):
    # CSVファイルを読み込む
    df = pd.read_csv(file_path)
    
    # 全データを使用
    x_data = df.iloc[:, 0].values  # x軸データ
    y_data = df.iloc[:, 1].values  # y軸データ

    # ファイル名からalphaの値を抽出（理論値用）
    filename = os.path.basename(file_path)
    theo_alpha1, theo_alpha2 = extract_alphas_from_filename(filename)

    try:
        # フィッティング手法の選択
        if FITTING_METHOD == FittingMethod.CURVE_FIT:
            result = fit_data(x_data, y_data)
            method_name = "Curve Fit"
        else:
            result = grid_search_fit(x_data, y_data)
            method_name = "Grid Search"
        
        # フィッティング結果のパラメータ
        fit_alpha1, fit_alpha2 = result.params

        # フィッティングの残差計算
        y_fitted = model_function(x_data, fit_alpha1, fit_alpha2)
        residual_fit = np.sum((y_data - y_fitted) ** 2)

        # 理論値の残差計算
        if theo_alpha1 is not None and theo_alpha2 is not None:
            y_theo = model_function(x_data, theo_alpha1, theo_alpha2)
            residual_theo = np.sum((y_data - y_theo) ** 2)

            # 理論値の方がフィッティングより良い場合は警告
            if residual_fit > residual_theo:
                print("\n" + "!"*50)
                print(f"警告: {filename}のフィッティングが理論値より悪い結果になっています！")
                print(f"フィッティング残差: {residual_fit:.6e}")
                print(f"理論値残差: {residual_theo:.6e}")
                print(f"フィッティングパラメータ: α₁={fit_alpha1:.3f}, α₂={fit_alpha2:.3f}")
                print(f"理論パラメータ: α₁={theo_alpha1:.3f}, α₂={theo_alpha2:.3f}")
                print("!"*50 + "\n")

        def create_plot(include_theoretical=True):
            # フィッティング結果をプロット
            plt.figure(figsize=(10, 6))
            ax = plt.gca()
            
            # 上と右の軸を追加し、副目盛りも表示
            ax.tick_params(direction='in', which='both', top=True, right=True)
            ax.tick_params(which='minor', top=True, right=True)  # 副目盛りを追加
            
            # データのプロット
            plt.scatter(x_data, y_data, marker='o', color='blue', 
                       label=f'Data $(\\alpha_1={theo_alpha1:.1f}, \\alpha_2={theo_alpha2:.1f})$', alpha=0.6)
            
            # フィッティング結果のプロット（理論線なしの場合のみ）
            y_fitted = model_function(x_data, fit_alpha1, fit_alpha2)
            # 係数の計算（fittings.pyのmodel_functionから）
            log_and_sqrt_coef_1 = calculate_intercept(alpha=fit_alpha1)
            log_and_sqrt_coef_2 = calculate_intercept(alpha=fit_alpha2)
            sqrt_coef_1 = np.exp(log_and_sqrt_coef_1)
            sqrt_coef_2 = np.exp(log_and_sqrt_coef_2)
            coef1 = sqrt_coef_1**2
            coef2 = sqrt_coef_2**2
            
            if not include_theoretical:  # 理論線なしの場合のみフィッティング線を表示
                plt.plot(x_data, y_fitted, linestyle='--', color='blue', 
                        label=f'Fit: $F_\\mathrm{{super}}^2 = {coef1:.4f}n^{{3-{fit_alpha1:.3f}}} + {coef2:.4f}n^{{3-{fit_alpha2:.3f}}}$')
            
            # 理論値のプロット（オプション）
            if include_theoretical and theo_alpha1 is not None and theo_alpha2 is not None:
                y_theo = model_function(x_data, theo_alpha1, theo_alpha2)
                plt.plot(x_data, y_theo, linestyle='-', color='red', linewidth=1.5,
                        label=f'Theoretical: $F_\\mathrm{{super}}^2(\\alpha_1={theo_alpha1:.1f}, \\alpha_2={theo_alpha2:.1f})$')
            
            # グラフの設定
            plt.xlabel('$n$', fontsize=16)
            plt.ylabel('$F_\\mathrm{super}^2$', fontsize=16)
            
            # タイトルを下部に配置
            title = f'Log-log plot of $n$-$F_\\mathrm{{super}}^2$ ($\\alpha_1={theo_alpha1:.1f}$, $\\alpha_2={theo_alpha2:.1f}$)'
            ax.text(0.5, -0.15, title, transform=ax.transAxes, 
                    fontsize=18, ha='center', va='center')
            
            # 凡例の設定
            plt.legend(fontsize=17, frameon=False)
            
            # 両軸を対数スケールに
            plt.xscale('log')
            plt.yscale('log')
            
            # x軸の範囲を設定
            plt.xlim([min(x_data), max(x_data)])
            
            return ax

        # グラフを保存するベースディレクトリの設定
        base_name = os.path.basename(file_path)
        output_dir = os.path.join(script_dir, "graphs")
        method_dir = "curve_fit" if FITTING_METHOD == FittingMethod.CURVE_FIT else "grid_search"
        
        # 理論線ありのグラフを保存
        with_theoretical_dir = os.path.join(output_dir, method_dir, "with_theoretical")
        os.makedirs(with_theoretical_dir, exist_ok=True)
        create_plot(include_theoretical=True)
        graph_file = os.path.join(with_theoretical_dir, f"fit_result_{os.path.splitext(base_name)[0]}.png")
        plt.savefig(graph_file, dpi=300, bbox_inches='tight', pad_inches=0.3)
        plt.close()

        # 理論線なしのグラフを保存
        without_theoretical_dir = os.path.join(output_dir, method_dir, "without_theoretical")
        os.makedirs(without_theoretical_dir, exist_ok=True)
        create_plot(include_theoretical=False)
        graph_file = os.path.join(without_theoretical_dir, f"fit_result_{os.path.splitext(base_name)[0]}.png")
        plt.savefig(graph_file, dpi=300, bbox_inches='tight', pad_inches=0.3)
        plt.close()
            
        print(f"Processed {base_name} using {method_name}:")
        print(f"  α₁={fit_alpha1:.4f}, α₂={fit_alpha2:.4f}, R²={result.r_squared:.4f}")
        if result.errors is not None:
            print(f"  Errors: Δα₁={alpha1_err:.4f}, Δα₂={alpha2_err:.4f}")
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