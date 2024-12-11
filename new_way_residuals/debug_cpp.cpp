#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

// Legacy手法：直線フィッティングによる残差計算
double get_residuals_legacy(const std::vector<double>& y) {
    int n = y.size();
    std::vector<double> x(n);
    for (int i = 0; i < n; ++i) {
        x[i] = i + 1.0;
    }

    double x_mean = 0.0;
    for (double val : x) {
        x_mean += val;
    }
    x_mean /= n;

    double y_mean = 0.0;
    for (double val : y) {
        y_mean += val;
    }
    y_mean /= n;

    double numerator = 0.0, denominator = 0.0;
    for (int i = 0; i < n; ++i) {
        numerator += (x[i] - x_mean) * (y[i] - y_mean);
        denominator += (x[i] - x_mean) * (x[i] - x_mean);
    }

    double slope = numerator / denominator;
    double intercept = y_mean - slope * x_mean;

    double residual_sum_squares = 0.0;
    for (int i = 0; i < n; ++i) {
        double residual = y[i] - (slope * x[i] + intercept);
        residual_sum_squares += residual * residual;
    }

    return residual_sum_squares / n;
}

// 直接手法：新しい残差計算方法
double get_residuals_direct(const std::vector<double>& y) {
    int n = y.size();
    if (n == 0) {
        return 0.0;
    }

    double sum = 0.0;
    for (double value : y) {
        sum += value;
    }
    double mean = sum / n;

    long double sq_sum = 0.0L;
    long double weighted_sum = 0.0L;
    for (size_t i = 0; i < y.size(); ++i) {
        double value = y[i];
        sq_sum += value * value;
        weighted_sum += (i+1) * value;
    }

    long double v2_y = (sq_sum / n) - mean*mean;
    long double second_term = (12.0L*(n+1)/(n-1))*std::pow((weighted_sum/n/(n+1) - mean/2),2);

    return v2_y - second_term;
}

int main() {
    // Pythonのテストデータと同じデータを使用
    std::vector<double> test_data = {
        0.37454012, 0.95071431, 0.73199394, 0.59865848, 0.15601864, 0.15599452,
        0.05808361, 0.86617615, 0.60111501, 0.70807258, 0.02058449, 0.96990985,
        0.83244264, 0.21233911, 0.18182497, 0.18340451, 0.30424224, 0.52475643,
        0.43194502, 0.29122914
    };

    // Legacy method
    double legacy_result = get_residuals_legacy(test_data);
    std::cout << std::fixed << std::setprecision(15);
    std::cout << "=== Legacy Method ===" << std::endl;
    std::cout << "Legacy result: " << legacy_result << std::endl << std::endl;

    // Direct method with debug info
    int n = test_data.size();
    double sum = 0.0;
    for (double value : test_data) {
        sum += value;
    }
    double mean = sum / n;

    long double sq_sum = 0.0L;
    long double weighted_sum = 0.0L;
    for (size_t i = 0; i < test_data.size(); ++i) {
        double value = test_data[i];
        sq_sum += value * value;
        weighted_sum += (i+1) * value;
    }

    std::cout << "=== Direct Method ===" << std::endl;
    std::cout << "n = " << n << std::endl;
    std::cout << "mean = " << mean << std::endl;
    std::cout << "sq_sum = " << sq_sum << std::endl;
    std::cout << "weighted_sum = " << weighted_sum << std::endl;

    long double v2_y = (sq_sum / n) - mean*mean;
    std::cout << "v2_y = " << v2_y << std::endl;

    long double second_term_part1 = 12.0L * (n+1)/(n-1);
    long double second_term_part2 = weighted_sum/n/(n+1) - mean/2;
    long double second_term = second_term_part1 * (second_term_part2 * second_term_part2);

    std::cout << "second_term_part1 = " << second_term_part1 << std::endl;
    std::cout << "second_term_part2 = " << second_term_part2 << std::endl;
    std::cout << "second_term = " << second_term << std::endl;

    double direct_result = v2_y - second_term;
    std::cout << "final result = " << direct_result << std::endl;

    std::cout << "\nDirect result: " << direct_result << std::endl;
    std::cout << "Difference: " << std::abs(legacy_result - direct_result) << std::endl;
    std::cout << "Relative difference: " << std::abs(legacy_result - direct_result) / legacy_result * 100 << "%" << std::endl;

    return 0;
}
