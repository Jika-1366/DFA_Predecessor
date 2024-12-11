import numpy as np

def print_debug_values(n=20):
    # Python version
    x_py = np.arange(1, n+1)
    y = np.random.rand(n)  # サンプルデータ
    
    print("Python version:")
    print("x values:", x_py)
    print("x mean:", np.mean(x_py))
    
    # C++ equivalent
    x_cpp = np.array([float(i) for i in range(1, n+1)])
    print("\nC++ equivalent:")
    print("x values:", x_cpp)
    print("x mean:", np.mean(x_cpp))
    
    # 比較
    print("\nDifference check:")
    print("Arrays equal:", np.array_equal(x_py, x_cpp))
    print("Means equal:", np.mean(x_py) == np.mean(x_cpp))

if __name__ == "__main__":
    print_debug_values()
