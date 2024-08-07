from sympy import symbols, integrate, expand, simplify, init_printing, Interval, sympify, Q, ask, Piecewise, Eq, Integer
from sympy.assumptions.assume import global_assumptions
from sympy.abc import x, y, alpha
import time

# シンボルの定義
n = symbols('n', integer=True, positive=True)  # nを正の整数として定義

# αの条件設定
alpha_condition = Interval.open(1, 2)
global_assumptions.add(Q.real(alpha))
global_assumptions.add(Q.is_true(alpha > 1))
global_assumptions.add(Q.is_true(alpha < 2))

# 項の定義（係数を削除）
terms = [
    y * x**(2 - alpha),
    x * y**(2 - alpha),
    x**(3 - alpha),
    y**(3 - alpha),
    y**(4 - 2*alpha),
    y**(2 - alpha) * (x - y),
    (x - y)**(3 - alpha),
    y**(1 - alpha) * (x - y)**2
]

# 共通因子
common_factor = (x - (n + 1)/2) * (y - (n + 1)/2)

# 結果を格納するリスト
results = []

# 各項の積分と結果の抽出
for i, term in enumerate(terms, 1):
    print(f"項 {i} の積分:")
    integrand = common_factor * term
    start_time = time.time()
    try:
        result = integrate(integrate(integrand, (y, 1, n)), (x, 1, n))
        end_time = time.time()
        print(f"積分結果: {result}")
        print(f"計算時間: {end_time - start_time:.2f} 秒")
        
        results.append(result)
    except Exception as e:
        print(f"エラーが発生しました: {e}")
    print("\n")

# 各項の結果を個別に表示
print("各項の積分結果:")
for i, result in enumerate(results, 1):
    print(f"項 {i}: {result}")

# 結果をテキストファイルに保存
with open('integration_results.txt', 'w') as f:
    for i, result in enumerate(results, 1):
        f.write(f"項 {i}: {result}\n\n")

print("\n各項の積分結果が'integration_results.txt'に保存されました。")

# α と n の条件確認
print(f"alpha は実数ですか？ {ask(Q.real(alpha))}")
print(f"alpha > 1 ですか？ {ask(Q.is_true(alpha > 1))}")
print(f"alpha < 2 ですか？ {ask(Q.is_true(alpha < 2))}")
print(f"n は整数ですか？ {ask(Q.integer(n))}")
print(f"n は正の数ですか？ {ask(Q.positive(n))}")