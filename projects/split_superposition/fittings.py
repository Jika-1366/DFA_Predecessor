import numpy as np
from scipy.optimize import curve_fit
import os
import sys
from typing import NamedTuple

# スクリプトの場所を取得
script_dir = os.path.dirname(os.path.abspath(__file__))
# プロジェクトルートを取得（2階層上）
project_root = os.path.dirname(os.path.dirname(script_dir))

# 相対パスとルートパスの両方を追加
sys.path.append(os.path.join(script_dir, "../../"))  # 相対パス
sys.path.append(project_root)  # プロジェクトルート

from formulas.calculate_intercept_0129 import calculate_intercept

class FitResult(NamedTuple):
    params: tuple[float, float]        # (alpha, beta)
    errors: tuple[float, float] | None # パラメータの標準誤差
    r_squared: float                   # 決定係数 R²

def calculate_r_squared(y_true: np.ndarray, y_pred: np.ndarray) -> float:
    """
    決定係数 R² を計算
    R² = 1 - (残差平方和 / 全変動)
    """
    ss_res = np.sum((y_true - y_pred) ** 2)
    ss_tot = np.sum((y_true - np.mean(y_true)) ** 2)
    
    # 分母が極めて小さい場合の処理を追加
    if np.abs(ss_tot) < 1e-10:  # 適切な閾値を設定
        return 0.0  # または float('nan')
    
    return 1 - (ss_res / ss_tot)

def model_function(x: np.ndarray, alpha: float, beta: float) -> np.ndarray:
    """
    フィッティングに使用するモデル関数
    
    Parameters:
    -----------
    x : np.ndarray
        xの値の配列
    alpha : float
        第1項の指数パラメータ
    beta : float
        第2項の指数パラメータ
        
    Returns:
    --------
    np.ndarray
        モデルによる予測値
    """
    log_and_sqrt_coef_1 = calculate_intercept(alpha=alpha)
    log_and_sqrt_coef_2 = calculate_intercept(alpha=beta)
    sqrt_coef_1 = np.exp(log_and_sqrt_coef_1)
    sqrt_coef_2 = np.exp(log_and_sqrt_coef_2)
    coef1 = sqrt_coef_1**2
    coef2 = sqrt_coef_2**2
    return coef1 * x**(3-alpha) + coef2 * x**(3-beta)

def fit_data(x_data: np.ndarray, y_data: np.ndarray, 
            initial_guess: list[float] = [1.2, 1.4], 
            bounds: tuple[list[float], list[float]] = ([1.1, 1.1], [1.98, 1.98])) -> FitResult:
    """
    scipy.optimize.curve_fitを使用してフィッティングを実行
    
    Parameters:
    -----------
    x_data : np.ndarray
        xの値の配列
    y_data : np.ndarray
        yの値の配列
    initial_guess : list[float]
        初期推測値 [alpha, beta]
    bounds : tuple[list[float], list[float]]
        パラメータの範囲 ([min_alpha, min_beta], [max_alpha, max_beta])
        
    Returns:
    --------
    FitResult
        フィッティング結果（パラメータ、誤差、決定係数）
    """
    params, covariance = curve_fit(model_function, x_data, y_data, p0=initial_guess, bounds=bounds)
    errors = tuple(np.sqrt(np.diag(covariance)))  # 標準誤差
    y_pred = model_function(x_data, *params)
    r2 = calculate_r_squared(y_data, y_pred)
    
    return FitResult(tuple(params), errors, r2)

def grid_search_fit(x_data: np.ndarray, y_data: np.ndarray, 
                   alpha_range: tuple[float, float] = (1.1, 1.98), 
                   beta_range: tuple[float, float] = (1.1, 1.98), 
                   step: float = 0.01) -> FitResult:
    """
    グリッドサーチによる最小二乗法フィッティング
    
    Parameters:
    -----------
    x_data : np.ndarray
        xの値の配列
    y_data : np.ndarray
        yの値の配列
    alpha_range : tuple[float, float]
        alphaの探索範囲 (min, max)
    beta_range : tuple[float, float]
        betaの探索範囲 (min, max)
    step : float
        グリッドの刻み幅
        
    Returns:
    --------
    FitResult
        フィッティング結果（パラメータ、誤差、決定係数）
    """
    alpha_values = np.arange(alpha_range[0], alpha_range[1] + step, step)
    beta_values = np.arange(beta_range[0], beta_range[1] + step, step)
    
    min_mse = float('inf')
    best_params = None
    
    for alpha in alpha_values:
        for beta in beta_values:
            y_pred = model_function(x_data, alpha, beta)
            mse = np.mean((y_data - y_pred) ** 2)
            
            if mse < min_mse:
                min_mse = mse
                best_params = (alpha, beta)
                best_y_pred = y_pred
    
    # グリッドサーチでは誤差の推定は行わない
    r2 = calculate_r_squared(y_data, best_y_pred)
    
    return FitResult(best_params, None, r2) 