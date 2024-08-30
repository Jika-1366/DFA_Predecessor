def calculate_cova2_inte(alpha):
    # b_inte1 の定義
    b_inte1 = (
    ( -192 + 196*alpha - 77*alpha**2 + 14*alpha**3 - alpha**4 ) 
    / 
    ( 24 * (-7+alpha) * (-6+alpha) * (-5+alpha) * (-4+alpha) * (-3+alpha) ))
    # b_inte2 の定義
    b_inte2 = (
    ( -38 + alpha*(38 + (-11+alpha)*alpha) ) 
    / 
    ( 4 * (-7+alpha) * (-6+alpha) * (-5+alpha) * (-4+alpha) * (-3+alpha) ))
    # b_inte3 の定義
    b_inte3 = (
    ( 12 - 7*alpha + alpha**2 ) 
    / 
    ( 4 * (-7+alpha) * (-6+alpha) * (-5+alpha) * (-4+alpha) ))
    # b_inte5 の定義
    b_inte5 = (
    ( 3 - alpha ) 
    / 
    ( 4 * (-7+alpha) * (-5+alpha) * (-4+alpha) ))
    
    # b_inte6 の定義
    b_inte6 = (
    ( -96 + 174*alpha - 60*alpha**2 + 6*alpha**3 ) 
    / 
    ( 24 * (-7+alpha) * (-6+alpha) * (-5+alpha) * (-3+alpha) * (-2+alpha) )
    )
    return b_inte1, b_inte2, b_inte3, b_inte5, b_inte6



if __name__ == "__main__":
    alpha = 1.9  # 例として alpha に 2 を代入
    b_inte1, b_inte2, b_inte3, b_inte5, b_inte6 = calculate_cova2_inte(alpha)
