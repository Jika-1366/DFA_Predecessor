import numpy as np
import matplotlib.pyplot as plt
import sys
import os
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
FITTING_METHOD = FittingMethod.GRID_SEARCH

def theoretical_curve(x, alpha):
    # 理論値の計算
    return x**alpha

def process_special_case():
    # 特別なケースのデータファイルを読み込む
    file_path = os.path.join(script_dir, "avg_dfa_alpha1.4_1.6.csv")
    df = pd.read_csv(file_path)
    
    # データの準備
    x_data = df.iloc[:, 0].values
    y_data = df.iloc[:, 1].values

    # 特別なケースの理論値
    theo_alpha1 = 1.41421  # √2
    theo_alpha2 = 1.6321206

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
        y_theo = model_function(x_data, theo_alpha1, theo_alpha2)
        residual_theo = np.sum((y_data - y_theo) ** 2)

        # 残差の比較
        if residual_fit > residual_theo:
            print("\n" + "!"*50)
            print(f"警告: フィッティングが理論値より悪い結果になっています！")
            print(f"フィッティング残差: {residual_fit:.6e}")
            print(f"理論値残差: {residual_theo:.6e}")
            print(f"フィッティングパラメータ: α₁={fit_alpha1:.6f}, α₂={fit_alpha2:.6f}")
            print(f"理論パラメータ: α₁={theo_alpha1:.6f}, α₂={theo_alpha2:.6f}")
            print("!"*50 + "\n")

        def create_plot(include_theoretical=True):
            plt.figure(figsize=(10, 6))
            ax = plt.gca()
            
            # 上と右の軸を追加し、副目盛りも表示
            ax.tick_params(direction='in', which='both', top=True, right=True)
            ax.tick_params(which='minor', top=True, right=True)
            
            # データのプロット
            plt.scatter(x_data, y_data, marker='o', color='blue', 
                       label=f'Data $(\\alpha_1={theo_alpha1:.6f}, \\alpha_2={theo_alpha2:.6f})$', alpha=0.6)
            
            # フィッティング結果のプロット
            y_fitted = model_function(x_data, fit_alpha1, fit_alpha2)
            # 係数の計算
            log_and_sqrt_coef_1 = calculate_intercept(alpha=fit_alpha1)
            log_and_sqrt_coef_2 = calculate_intercept(alpha=fit_alpha2)
            sqrt_coef_1 = np.exp(log_and_sqrt_coef_1)
            sqrt_coef_2 = np.exp(log_and_sqrt_coef_2)
            coef1 = sqrt_coef_1**2
            coef2 = sqrt_coef_2**2
            
            if not include_theoretical:
                plt.plot(x_data, y_fitted, linestyle='--', color='blue', 
                        label=f'Fit: $F_\\mathrm{{super}}^2 = {coef1:.4f}n^{{3-{fit_alpha1:.4f}}} + {coef2:.4f}n^{{3-{fit_alpha2:.4f}}}$')
            
            # 理論値のプロット
            if include_theoretical:
                y_theo = model_function(x_data, theo_alpha1, theo_alpha2)
                plt.plot(x_data, y_theo, linestyle='-', color='red', linewidth=1.5,
                        label=f'Theoretical: $F_\\mathrm{{super}}^2(\\alpha_1={theo_alpha1:.6f}, \\alpha_2={theo_alpha2:.6f})$')
            
            # グラフの設定
            plt.xlabel('$n$', fontsize=16)
            plt.ylabel('$F_\\mathrm{super}^2$', fontsize=16)
            
            # タイトルを下部に配置
            title = f'Log-log plot of $n$-$F_\\mathrm{{super}}^2$ (Special Case)'
            ax.text(0.5, -0.15, title, transform=ax.transAxes, 
                    fontsize=18, ha='center', va='center')
            
            plt.legend(fontsize=12, frameon=False)
            plt.xscale('log')
            plt.yscale('log')
            plt.xlim([min(x_data), max(x_data)])
            
            return ax

        # グラフを保存
        # 理論線ありのグラフを保存
        create_plot(include_theoretical=True)
        plt.savefig(os.path.join(script_dir, "special_case_with_theoretical.png"), 
                   dpi=300, bbox_inches='tight', pad_inches=0.3)
        plt.close()

        # 理論線なしのグラフを保存
        create_plot(include_theoretical=False)
        plt.savefig(os.path.join(script_dir, "special_case_without_theoretical.png"), 
                   dpi=300, bbox_inches='tight', pad_inches=0.3)
        plt.close()
            
        print(f"特別なケースの処理が完了しました（{method_name}使用）:")
        print(f"  α₁={fit_alpha1:.6f}, α₂={fit_alpha2:.6f}, R²={result.r_squared:.6f}")
        if result.errors is not None:
            print(f"  Errors: Δα₁={result.errors[0]:.6f}, Δα₂={result.errors[1]:.6f}")
        return True
    except Exception as e:
        print(f"エラーが発生しました: {str(e)}")
        return False

if __name__ == "__main__":
    process_special_case() 