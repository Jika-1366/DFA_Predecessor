from formulas.covariance1 import calculate_second_cova1_coff1, calculate_second_cova1_inte1
from formulas.a_and_b import calculate_a_and_b, calculate_pc, calculate_qc, calculate_c, calculate_mu


#二番目に大きいサイズの項n^(4-2alpha)からを計算します。

def calculate_second_cova1(alpha, qc,pc,mu):
    second_cova1_coffi = calculate_second_cova1_coff1(qc, pc,mu)
    second_cova1_inte = calculate_second_cova1_inte1(alpha, qc, pc, mu)
    return second_cova1_inte*second_cova1_coffi


def calculate_second_cova2(alpha, qc,pc,mu, a_and_b):
    second_cova2_inte =  (
        (10 - 9*alpha + 2*alpha**2)
        /
        (8 * (-4+alpha) * (-3+alpha) * (-7+2*alpha) * (-5+2*alpha))
    )
    second_cova2_coffi = a_and_b["b4"]
    return second_cova2_inte*second_cova2_coffi

def calculate_second_cova3(alpha, qc,pc,mu, a_and_b):
    second_cova3_inte = (
         1 / (2 * (-3 + alpha) * (-5 + 2*alpha))
    )
    second_cova3_coffi = a_and_b["b4"]
    return second_cova3_inte*second_cova3_coffi

def calculate_second_coffi(alpha):
    tau_0 = 1.0
    c = calculate_c(alpha=alpha, tau_0=tau_0)
    mu = calculate_mu(alpha= alpha, tau_0 = tau_0)
    pc = calculate_pc(tau_0=tau_0, alpha=alpha, mu=mu)
    qc = calculate_qc(tau_0=tau_0, alpha=alpha, mu=mu)
    a_and_b= calculate_a_and_b(c, tau_0, alpha, mu)


    sum_coffi1 = calculate_second_cova1(alpha, qc,pc,mu)
    sum_coffi2 = calculate_second_cova2(alpha, qc,pc,mu, a_and_b)
    sum_coffi3 = calculate_second_cova3(alpha, qc,pc,mu, a_and_b)
    coffi1 = sum_coffi1
    coffi2= -sum_coffi2/12
    coffi3 = -sum_coffi3
    coffi = coffi1 + coffi2 + coffi3
    return coffi
    

