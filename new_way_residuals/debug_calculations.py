import numpy as np

def get_residuals_legacy(y):
    n = len(y)
    x = np.arange(1, n+1)
    
    x_mean = np.mean(x)
    y_mean = np.mean(y)
    
    numerator = np.sum((x - x_mean) * (y - y_mean))
    denominator = np.sum((x - x_mean) ** 2)
    
    slope = numerator / denominator
    intercept = y_mean - slope * x_mean
    
    y_pred = slope * x + intercept
    residuals = y - y_pred
    return np.sum(residuals ** 2)/n

def get_residuals_direct(y):
    n = len(y)
    mean = np.mean(y)
    
    # 中間計算値を表示
    print(f"n = {n}")
    print(f"mean = {mean}")
    
    sq_sum = np.sum(y * y)
    print(f"sq_sum = {sq_sum}")
    
    weighted_sum = np.sum([(i+1) * val for i, val in enumerate(y)])
    print(f"weighted_sum = {weighted_sum}")
    
    v2_y = (sq_sum / n) - mean * mean
    print(f"v2_y = {v2_y}")
    
    second_term_part1 = 12.0 * (n+1)/(n-1)
    second_term_part2 = weighted_sum/n/(n+1) - mean/2
    second_term = second_term_part1 * (second_term_part2 ** 2)
    
    print(f"second_term_part1 = {second_term_part1}")
    print(f"second_term_part2 = {second_term_part2}")
    print(f"second_term = {second_term}")
    
    result = v2_y - second_term
    print(f"final result = {result}")
    
    return result

if __name__ == "__main__":
    # テストデータの生成（再現性のため乱数シードを固定）
    np.random.seed(42)
    test_data = np.random.rand(20)
    
    print("=== Legacy Method ===")
    legacy_result = get_residuals_legacy(test_data)
    print(f"Legacy result: {legacy_result}\n")
    
    print("=== Direct Method ===")
    direct_result = get_residuals_direct(test_data)
    print(f"\nDirect result: {direct_result}")
    
    print(f"\nDifference: {abs(legacy_result - direct_result)}")
    print(f"Relative difference: {abs(legacy_result - direct_result) / legacy_result * 100}%")
    
    # テストデータの値も表示
    print("\nTest data:")
    print(test_data)
