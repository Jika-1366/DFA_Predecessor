import sympy as sp

# Define symbols
x, y, n, alpha = sp.symbols('x y n alpha')

# Define the integrand
integrand = (x - y)**(3 - alpha) * (x - (n + 1)/2) * (y - (n + 1)/2)

# Perform the double integral
result = sp.integrate(integrand, (x, y, n), (y, 1, n))

# Factor the result
factored_result = sp.factor(result)

# Print the result
print(factored_result)

# If you want to substitute specific values for alpha and n:
# alpha_val = 1.5
# n_val = 10
# substituted_result = factored_result.subs({alpha: alpha_val, n: n_val})
# print(substituted_result)