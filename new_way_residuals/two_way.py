import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression

# Data
x = np.array([-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5]).reshape(-1, 1)
y = np.array([1, 1.5, 1.9, 2.6, 3.0, 3.2, 3.8, 3.9, 4.2, 4.6, 5.0])

# Linear Regression
model = LinearRegression()
model.fit(x, y)

# Predicted values
y_pred = model.predict(x)
print("予測値:", y_pred)

# Method 1: Sum of Squared Residuals (Ordinary Method)
residuals = y - y_pred
sum_squared_residuals = np.sum(residuals**2)
print("残差:", residuals)

# Method 2: Custom Formula for Residual Calculation
n = len(y)
y_average = np.mean(y)
print("yの平均値:", y_average)

# Calculating the "variance-like" value based on the given corrected formula
sum_y_squared = np.sum(y**2)
average_contribution = n * (y_average ** 2)
value_based_on_sum = sum_y_squared - average_contribution
print("分散のような値:", value_based_on_sum)

# Calculating the term in the custom formula
sum_i_y = sum((i + 1) * y[i] for i in range(n))
custom_slope_squared = (sum_i_y - y_average * (n * (n + 1) / 2)) ** 2
print("カスタム傾き二乗:", custom_slope_squared)

# Custom residual sum calculation
custom_residual_sum = value_based_on_sum - (12 / ((n - 1) * n * (n + 1))) * custom_slope_squared

# Displaying both residual sums
print("通常の残差二乗和:", sum_squared_residuals)
print("カスタム残差和:", custom_residual_sum)
