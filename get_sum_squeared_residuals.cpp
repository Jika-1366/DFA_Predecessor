#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <iomanip>
#include "power_law_tool_equ.hpp"

using namespace std;

double test_get_sum_squared_residuals(const vector<double>& y) {
    cout << "テストデータ: ";
    for (const auto& val : y) {
        cout << val << " ";
    }
    cout << endl;

    // get_directly_residuls2を呼び出し
    vector<unsigned int> y_uint(y.begin(), y.end());
    double result = get_directly_residuls2(y_uint);

    cout << "結果: " << result << endl;

    return result;
}

double calculate_sum_squared_residuals(const vector<double>& x, const vector<double>& y) {
    int n = y.size();

    // Calculate y_average
    double y_average = accumulate(y.begin(), y.end(), 0.0) / n;

    // Calculating the "variance-like" value
    double sum_y_squared = 0.0;
    for (double val : y) {
        sum_y_squared += val * val;
    }
    double average_contribution = n * (y_average * y_average);
    double value_based_on_sum = sum_y_squared - average_contribution;

    // Calculating the term in the custom formula
    double sum_i_y = 0.0;
    for (int i = 0; i < n; ++i) {
        sum_i_y += (i + 1) * y[i];
    }
    double custom_slope_squared = pow(sum_i_y - y_average * (n * (n + 1) / 2), 2);

    // Custom residual sum calculation
    double custom_residual_sum = value_based_on_sum - (12.0 / ((n - 1) * n * (n + 1))) * custom_slope_squared;

    return custom_residual_sum;
}

void test_residual_calculations(const vector<double>& y) {
    // x値の生成（-5から5までの11個の値）
    vector<double> x(11);
    for (int i = 0; i < 11; ++i) {
        x[i] = i - 5;
    }

    // カスタムの残差計算
    double custom_residual_sum = calculate_sum_squared_residuals(x, y);

    // 結果の表示
    cout << fixed << setprecision(10);
    cout << "カスタムの残差和: " << custom_residual_sum << endl;
}

int main() {
    // 11個のテストデータの作成
    vector<double> y = {1.0, 1.5, 1.9, 2.6, 3.0, 3.2, 3.8, 3.9, 4.2, 4.6, 5.0};

    // 両方のテスト関数を同じデータで呼び出し
    test_get_sum_squared_residuals(y);
    test_residual_calculations(y);

    return 0;
}