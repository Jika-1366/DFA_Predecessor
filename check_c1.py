# 計算値を計算する関数
def check_c1(alpha):
    
    mu = alpha / (alpha - 1)
    secondmoment = alpha/(alpha-2)

    sigma2 = alpha/((mu**3)*(alpha-2)*((alpha-1)**2))
    #c1 = (secondmoment - 2*(mu**2))/(2*(mu**2))
    c1 = (1+2*alpha-alpha**2)/(2*alpha*(alpha-2))
    
    print(f"c1: {c1}")
# alpha = 1.1から2.0まで0.1刻みで実行
for alpha in [1.1 + i * 0.1 for i in range(9)]:
    print(f"\nalpha = {alpha}")
    check_c1(alpha)