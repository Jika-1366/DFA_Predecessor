

#積分近似は2つだけ。
def calculate_cova1_inte1(alpha):
    cova1inte1 = 1 / (4 - alpha)    
    return cova1inte1

def calculate_cova1_coffi1(qc, pc,mu):
    cova1_coffi1 = qc
    return cova1_coffi1

def calculate_second_cova1_coff1(qc, pc,mu):
    second_cova1_coffi1 = pc**2
    return second_cova1_coffi1