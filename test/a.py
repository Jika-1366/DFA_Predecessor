import numpy as np
from scipy import integrate

def integrand(t, u, s, alpha):
    return alpha * (s + t*(u-s))**(alpha-1)

def calculate_both_sides(u, s, alpha):
    # 左辺の計算
    left_side = u**alpha - s**alpha
    
    # 右辺の積分計算
    integral, _ = integrate.quad(integrand, 0, 1, args=(u, s, alpha))
    right_side = (u-s) * integral
    
    return left_side, right_side

def check_equation(u, s, alpha):
    left, right = calculate_both_sides(u, s, alpha)
    diff = abs(left - right)
    is_equal = diff < 1e-10
    
    print(f"\nテストケース:")
    print(f"α = {alpha:.2f}")
    print(f"u = {u:.2f}, s = {s:.2f}")
    print(f"左辺: {left:.10f}")
    print(f"右辺: {right:.10f}")
    print(f"差分: {diff:.10f}")
    print(f"結果: {'一致' if is_equal else '不一致'}")
    
    return is_equal

# 複数のテストケースを実行
print("=== 方程式の検証 ===")

# テストケース1: 基本的なケース
alphas = [1.1, 1.5, 1.9]  # αの値
u_values = [2.0, 3.0, 4.0]  # uの値
s_values = [1.0, 1.5, 2.0]  # sの値

total_tests = 0
passed_tests = 0

for alpha in alphas:
    for u in u_values:
        for s in s_values:
            if s < u:  # s < u の場合のみテスト
                total_tests += 1
                if check_equation(u, s, alpha):
                    passed_tests += 1

print("\n=== テスト結果サマリー ===")
print(f"総テスト数: {total_tests}")
print(f"成功数: {passed_tests}")
print(f"成功率: {(passed_tests/total_tests)*100:.2f}%")
