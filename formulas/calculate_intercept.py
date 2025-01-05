import math

def calculate_coef1(c, mu, alpha):
  """
  画像1の数式を計算する関数

  Args:
    c: 係数 c
    mu: 係数 μ
    alpha: 係数 α

  Returns:
    計算結果
  """
  return (2.0 * c) / (mu**3 * math.gamma(5 - alpha))

def calculate_coef2(c, mu, alpha):
  """
  画像3の数式を計算する関数

  Args:
    c: 係数 c
    mu: 係数 μ
    alpha: 係数 α

  Returns:
    計算結果
  """
  return c * ( ((-3.0 + alpha)/math.gamma(6.0 - alpha)) + 2*(-63.0 + 86*alpha - 25*alpha**2 + 2*alpha**3)/math.gamma(8.0 - alpha) ) / (4 * (-7.0 + alpha) * mu**3)

def calculate_coef3_same_result(c, mu, alpha):
  """
  画像2の数式を計算する関数

  Args:
    c: 係数 c
    mu: 係数 μ
    alpha: 係数 α

  Returns:
    計算結果
  """
  return (c / (mu**3))* ( -1/((-5.0 + alpha) * math.gamma(5.0 - alpha)) + 2/math.gamma(6.0 - alpha) ) 


def calculate_coef3(c, mu, alpha):
  """
  更新された画像2の数式を計算する関数

  Args:
    c: 係数 c
    mu: 係数 μ
    alpha: 係数 α

  Returns:
    計算結果
  """
  gamma2 = math.gamma(2 - alpha)
  gamma3 = math.gamma(3 - alpha)
  gamma4 = math.gamma(4 - alpha)
  numerator = c * (gamma3 * gamma4 + gamma2 * (-gamma4 + gamma3 * (-3.0 + alpha)) * (-2.0 + alpha))
  denominator = gamma2 * gamma3 * gamma4 * (-5.0 + alpha) * (-4.0 + alpha) * (-3.0 + alpha) * (-2.0 + alpha) * (mu**3)
  return numerator / denominator



from formulas.a_and_b import calculate_c, calculate_mu
import numpy as np
def calculate_intercept(alpha):
    if alpha >=1.99:
        return None
    tau_0 = 1.0
    c = calculate_c(alpha=alpha, tau_0=tau_0)
    mu = calculate_mu(alpha=alpha, tau_0=tau_0)

    coef1 = calculate_coef1(c=c, mu=mu, alpha=alpha)
    coef2 = calculate_coef2(c=c, mu=mu, alpha=alpha)
    coef3 = calculate_coef3(c=c, mu=mu, alpha=alpha)

    intercept = coef1 -12.0* 2.0*coef2 -2.0* coef3
    if intercept < 0:
        print(f"警告: alpha={alpha}のとき、intercept={intercept}は負の値です。")
        print(f"coef1={coef1}, coef2={coef2}, coef3={coef3}")
    sqrt_intercept = np.sqrt(intercept)
    return np.log(sqrt_intercept)


def calculate_second_intercept(alpha):
   
    """
    第二項の切片を計算する関数

    Args:
        alpha: 係数 α

    Returns:
        計算結果
    """
    tau_0 = 1.0
    c = calculate_c(alpha=alpha, tau_0=tau_0)
    mu = calculate_mu(alpha=alpha, tau_0=tau_0)


    coef1 = -1/(2.0*mu)

    numerator = c * (-2520.0 + 2754.0*alpha - 1175.0*alpha**2 + 245.0*alpha**3 - 25.0*alpha**4 + alpha**5)
    denominator = 48.0 * mu**3 * math.gamma(8.0 - alpha)
    first_term = 1 / (240.0 * mu)
    second_term = numerator / denominator
    coef2 = -first_term - second_term


    numerator = c * (-60.0 + 47.0*alpha - 12.0*alpha**2 + alpha**3)
    denominator = 6.0 * (mu**3) * math.gamma(6.0 - alpha)
    coef3 = -1/(6.0*mu) + numerator/denominator
    

    return coef1-12.0*2.0*coef2 -2.0*coef3


