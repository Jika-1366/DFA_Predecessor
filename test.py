import math
from formulas.calculate_intercept import calculate_c, calculate_mu, calculate_coef2, calculate_coef2_same_result

if __name__ == "__main__":
    # alpha = 1.5での検証
    alpha = 1.5
    tau_0 = 1.0
    c = calculate_c(alpha=alpha, tau_0=tau_0)
    mu = calculate_mu(alpha=alpha, tau_0=tau_0)

    # 2つの関数の結果を比較
    result1 = calculate_coef2(c=c, mu=mu, alpha=alpha)
    result2 = calculate_coef2_same_result(c=c, mu=mu, alpha=alpha)

    print(f"calculate_coef2の結果: {result1:.10f}")
    print(f"calculate_coef2_same_resultの結果: {result2:.10f}")
    print(f"差分: {abs(result1 - result2):.10f}")
