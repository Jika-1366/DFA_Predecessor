import math
from scipy.special import gamma

def calculate_mu(alpha, tau_0):
    return (alpha * tau_0) / (alpha - 1)

def calculate_pc(tau_0, alpha, mu):
    return (tau_0**alpha) / ((alpha - 1) * (2 - alpha) * mu**2)

def calculate_qc(tau_0, alpha, mu):
    return 2 * (tau_0**alpha) / ((2 - alpha) * (3 - alpha) * mu**3)


def calculate_c(alpha, tau_0):
    return abs(gamma(1 - alpha)) * (tau_0 ** alpha)

def calculate_a_and_b(c, tau_0, alpha, mu):
    pc = calculate_pc(tau_0, alpha, mu)
    qc = calculate_qc(tau_0, alpha, mu)
    
    a1 = a2 = -pc / mu
    a3 = qc
    a4 = 2 * pc / mu
    a5 = pc**2
    a6 = 2 * c / (mu**3 * math.gamma(3 - alpha))
    a7 = -c / (mu**3 * math.gamma(4 - alpha))
    a8 = c / (2 * mu**3 * math.gamma(2 - alpha))
    
    b1 = a1
    b2 = a2 + a6 - 2*a8
    b3 = a3 + a4 - a6 + a8
    b4 = a5
    b5 = a7
    b6 = a8
    
    return {
        'a1': a1, 'a2': a2, 'a3': a3, 'a4': a4, 'a5': a5, 'a6': a6, 'a7': a7, 'a8': a8,
        'b1': b1, 'b2': b2, 'b3': b3, 'b4': b4, 'b5': b5, 'b6': b6
    }

if __name__ == "__main__":
    # 使用例
    c = 1.0
    tau_0 = 1.0
    alpha = 1.5
    mu = (alpha * tau_0) / (alpha - 1)


    a_and_b = calculate_a_and_b(c, tau_0, alpha, mu)

    print(a_and_b)
