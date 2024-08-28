from sympy import symbols, integrate, expand, simplify, init_printing
from sympy.abc import x, y, n, alpha

# Define symbols
a1, a2, a3, a4, a5, a6, a7, a8 = symbols('a1 a2 a3 a4 a5 a6 a7 a8')

# Define the integrand
integrand = (x - (n + 1)/2) * (y - (n + 1)/2) * (
    a1 * y * x**(2 - alpha) + 
    a2 * x * y**(2 - alpha) + 
    a3 * x**(3 - alpha) + 
    a4 * y**(3 - alpha) + 
    a5 * y**(4 - 2*alpha) + 
    a6 * y**(2 - alpha) * (x - y) + 
    a7 * (x - y)**(3 - alpha) + 
    a8 * y**(1 - alpha) * (x - y)**2
)

# Perform the double integral
result = integrate(integrate(integrand, (y, 1, n)), (x, 1, n))

# Simplify the result
simplified_result = simplify(expand(result))

print("Simplified Integral Result:")
print(simplified_result)