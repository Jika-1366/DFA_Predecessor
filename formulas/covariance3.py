def calculate_cova3_inte(alpha):
    cova3inte1 = (3 - alpha) / (2 * (-5 + alpha) * (-3 + alpha))
    cova3inte2 = 1 / (15 - 8 * alpha + alpha**2)
    cova3inte3 = 1 / (20 - 9 * alpha + alpha**2)
    cova3inte5 = 1 / (20 - 9 * alpha + alpha**2)
    cova3inte6 = 1 / (10 - 7 * alpha + alpha**2)
    return cova3inte1, cova3inte2, cova3inte3, cova3inte5, cova3inte6

