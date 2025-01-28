import scipy.special
import math

def cova1(c, alpha, mu):
    """
    Calculates the value based on the first expression.

    Expression:
    (c * (-3 + alpha)) / (4 * (-7 + alpha) * (mu**3) * scipy.special.gamma(6 - alpha))

    Args:
        c (float): Value of c.
        alpha (float): Value of alpha (α).
        mu (float): Value of mu (μ).

    Returns:
        float: The calculated value. Returns float('inf') if denominator is zero.
    """
    numerator = -2.0*c
    denominator =  (-4 + alpha) * (mu**3) * scipy.special.gamma(4 - alpha)
    if denominator == 0:
        return float('inf')  # Or handle division by zero as needed
    return numerator / denominator

def cova2(c, alpha, mu):
    """
    Calculates the value based on the second expression (which is identical to the first in the prompt).

    Expression:
    (c * (-3 + alpha)) / (4 * (-7 + alpha) * (mu**3) * scipy.special.gamma(6 - alpha))

    Args:
        c (float): Value of c.
        alpha (float): Value of alpha (α).
        mu (float): Value of mu (μ).

    Returns:
        float: The calculated value. Returns float('inf') if denominator is zero.
    """
    numerator = c * (-3 + alpha)
    denominator = 4 * (-7 + alpha) * (mu**3) * scipy.special.gamma(6 - alpha)
    if denominator == 0:
        return float('inf')  # Or handle division by zero as needed
    return numerator / denominator


def cova3(c, alpha, mu):
    """
    Calculates the value based on the third expression.

    Expression:
    - (c * (-4 + alpha)) / ((mu**3) * scipy.special.gamma(6 - alpha))

    Args:
        c (float): Value of c.
        alpha (float): Value of alpha (α).
        mu (float): Value of mu (μ).

    Returns:
        float: The calculated value. Returns float('inf') if denominator is zero.
    """
    numerator = -c * (-4 + alpha)
    denominator = (mu**3) * scipy.special.gamma(6 - alpha)
    if denominator == 0:
        return float('inf')  # Or handle division by zero as needed
    return numerator / denominator



from formulas.a_and_b import calculate_c, calculate_mu
import numpy as np
def calculate_intercept(alpha):
    if alpha >=1.99:
        return None
    tau_0 = 1.0
    c = calculate_c(alpha=alpha, tau_0=tau_0)
    mu = calculate_mu(alpha=alpha, tau_0=tau_0)

    coef1 = cova1(c=c, mu=mu, alpha=alpha)
    coef2 = cova2(c=c, mu=mu, alpha=alpha)
    coef3 = cova3(c=c, mu=mu, alpha=alpha)

    intercept = coef1 -12.0* 2.0*coef2 -2.0* coef3
    if intercept < 0:
        print(f"警告: alpha={alpha}のとき、intercept={intercept}は負の値です。")
        print(f"coef1={coef1}, coef2={coef2}, coef3={coef3}")
    sqrt_intercept = np.sqrt(intercept)
    return np.log(sqrt_intercept)


if __name__ == '__main__':
    # Example usage:
    c_val = 1.0
    alpha_val = 1.5
    mu_val = 2.0

    result1 = cova1(c_val, alpha_val, mu_val)
    result2 = cova2(c_val, alpha_val, mu_val)
    result3 = cova3(c_val, alpha_val, mu_val)

    print(f"cova1 result: {result1}")
    print(f"cova2 result: {result2}")
    print(f"cova3 result: {result3}")