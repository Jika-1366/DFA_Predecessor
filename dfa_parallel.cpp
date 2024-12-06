#include <vector>
#include <iostream>
#include <numeric>
#include <execution>
#include <chrono>
#include <omp.h>
#include <thread>
#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <immintrin.h>
#include "power_law_tool.hpp"

// 基本実装（変更なし）
double get_directly_residuals2_basic(const std::vector<double>& y) {
    int n = y.size();
    std::vector<double> x(n);
    std::iota(x.begin(), x.end(), 1.0);

    double x_mean = std::accumulate(x.begin(), x.end(), 0.0) / n;
    double y_mean = std::accumulate(y.begin(), y.end(), 0.0) / n;

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

    return residual_sum_squares;
}

// 改善されたOpenMP実装
double get_directly_residuals2_openmp(const std::vector<double>& y) {
    int n = y.size();
    if (n < 10000) return get_directly_residuals2_basic(y);

    std::vector<double> x(n);
    
    // データの事前計算を一度のループで行う
    double x_sum = 0.0, y_sum = 0.0;
    double x_squared_sum = 0.0, xy_sum = 0.0;
    
    #pragma omp parallel
    {
        double local_x_sum = 0.0, local_y_sum = 0.0;
        double local_x_squared_sum = 0.0, local_xy_sum = 0.0;
        
        #pragma omp for nowait
        for (int i = 0; i < n; ++i) {
            double x_val = static_cast<double>(i + 1);
            x[i] = x_val;
            
            local_x_sum += x_val;
            local_y_sum += y[i];
            local_x_squared_sum += x_val * x_val;
            local_xy_sum += x_val * y[i];
        }
        
        #pragma omp critical
        {
            x_sum += local_x_sum;
            y_sum += local_y_sum;
            x_squared_sum += local_x_squared_sum;
            xy_sum += local_xy_sum;
        }
    }

    double x_mean = x_sum / n;
    double y_mean = y_sum / n;
    
    // 線形回帰の係数を直接計算
    double slope = (n * xy_sum - x_sum * y_sum) / (n * x_squared_sum - x_sum * x_sum);
    double intercept = (y_sum - slope * x_sum) / n;

    // 残差計算を並列化
    double residual_sum_squares = 0.0;
    
    #pragma omp parallel reduction(+:residual_sum_squares)
    {
        #pragma omp for schedule(static)
        for (int i = 0; i < n; ++i) {
            double predicted = slope * x[i] + intercept;
            double residual = y[i] - predicted;
            residual_sum_squares += residual * residual;
        }
    }

    return residual_sum_squares;
}

// 改善されたSIMD実装
double get_directly_residuals2_simd(const std::vector<double>& y) {
    int n = y.size();
    if (n < 1000 || n % 4 != 0) return get_directly_residuals2_basic(y);

    std::vector<double> x(n);
    std::iota(x.begin(), x.end(), 1.0);

    // SIMD用の累積変数
    __m256d sum_x = _mm256_setzero_pd();
    __m256d sum_y = _mm256_setzero_pd();
    __m256d sum_xy = _mm256_setzero_pd();
    __m256d sum_xx = _mm256_setzero_pd();

    // 一度のループで必要な統計量を計算
    #pragma omp parallel
    {
        __m256d local_sum_x = _mm256_setzero_pd();
        __m256d local_sum_y = _mm256_setzero_pd();
        __m256d local_sum_xy = _mm256_setzero_pd();
        __m256d local_sum_xx = _mm256_setzero_pd();

        #pragma omp for nowait
        for (int i = 0; i < n; i += 4) {
            __m256d x_vec = _mm256_loadu_pd(&x[i]);
            __m256d y_vec = _mm256_loadu_pd(&y[i]);
            
            local_sum_x = _mm256_add_pd(local_sum_x, x_vec);
            local_sum_y = _mm256_add_pd(local_sum_y, y_vec);
            local_sum_xy = _mm256_add_pd(local_sum_xy, _mm256_mul_pd(x_vec, y_vec));
            local_sum_xx = _mm256_add_pd(local_sum_xx, _mm256_mul_pd(x_vec, x_vec));
        }

        #pragma omp critical
        {
            sum_x = _mm256_add_pd(sum_x, local_sum_x);
            sum_y = _mm256_add_pd(sum_y, local_sum_y);
            sum_xy = _mm256_add_pd(sum_xy, local_sum_xy);
            sum_xx = _mm256_add_pd(sum_xx, local_sum_xx);
        }
    }

    // 水平加算
    double x_sum = 0.0, y_sum = 0.0, xy_sum = 0.0, xx_sum = 0.0;
    double temp[4];

    _mm256_storeu_pd(temp, sum_x);
    x_sum = temp[0] + temp[1] + temp[2] + temp[3];
    
    _mm256_storeu_pd(temp, sum_y);
    y_sum = temp[0] + temp[1] + temp[2] + temp[3];
    
    _mm256_storeu_pd(temp, sum_xy);
    xy_sum = temp[0] + temp[1] + temp[2] + temp[3];
    
    _mm256_storeu_pd(temp, sum_xx);
    xx_sum = temp[0] + temp[1] + temp[2] + temp[3];

    // 線形回帰係数の計算
    double slope = (n * xy_sum - x_sum * y_sum) / (n * xx_sum - x_sum * x_sum);
    double intercept = (y_sum - slope * x_sum) / n;

    // 残差計算（SIMD + OpenMP）
    double final_sum = 0.0;
    
    #pragma omp parallel reduction(+:final_sum)
    {
        __m256d local_sum = _mm256_setzero_pd();
        __m256d slope_vec = _mm256_set1_pd(slope);
        __m256d intercept_vec = _mm256_set1_pd(intercept);
        
        #pragma omp for nowait
        for (int i = 0; i < n; i += 4) {
            __m256d x_vec = _mm256_loadu_pd(&x[i]);
            __m256d y_vec = _mm256_loadu_pd(&y[i]);
            __m256d predicted = _mm256_add_pd(_mm256_mul_pd(slope_vec, x_vec), intercept_vec);
            __m256d residual = _mm256_sub_pd(y_vec, predicted);
            local_sum = _mm256_add_pd(local_sum, _mm256_mul_pd(residual, residual));
        }
        
        double temp[4];
        _mm256_storeu_pd(temp, local_sum);
        double thread_sum = temp[0] + temp[1] + temp[2] + temp[3];
        final_sum += thread_sum;
    }

    return final_sum;
}

// std::execution実装（変更なし）
double get_directly_residuals2_execution(const std::vector<double>& y) {
    int n = y.size();
    std::vector<double> x(n);
    std::iota(x.begin(), x.end(), 1.0);

    if (n < 10000) {
        return get_directly_residuals2_basic(y);
    }

    double x_mean = std::reduce(std::execution::par, x.begin(), x.end()) / n;
    double y_mean = std::reduce(std::execution::par, y.begin(), y.end()) / n;

    std::vector<double> temp_num(n), temp_den(n);
    std::transform(std::execution::par, x.begin(), x.end(), y.begin(), temp_num.begin(),
        [x_mean, y_mean](double x_val, double y_val) {
            return (x_val - x_mean) * (y_val - y_mean);
        });
    std::transform(std::execution::par, x.begin(), x.end(), temp_den.begin(),
        [x_mean](double x_val) {
            double diff = x_val - x_mean;
            return diff * diff;
        });

    double numerator = std::reduce(std::execution::par, temp_num.begin(), temp_num.end());
    double denominator = std::reduce(std::execution::par, temp_den.begin(), temp_den.end());

    double slope = numerator / denominator;
    double intercept = y_mean - slope * x_mean;

    std::vector<double> residuals(n);
    std::transform(std::execution::par, x.begin(), x.end(), y.begin(), residuals.begin(),
        [slope, intercept](double x_val, double y_val) {
            double residual = y_val - (slope * x_val + intercept);
            return residual * residual;
        });

    return std::reduce(std::execution::par, residuals.begin(), residuals.end());
}

void benchmark_residuals(const std::vector<double>& data) {
    std::cout << "データサイズ: " << data.size() << "点\n\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    double result_basic = get_directly_residuals2_basic(data);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_basic = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    double result_openmp = get_directly_residuals2_openmp(data);
    end = std::chrono::high_resolution_clock::now();
    auto duration_openmp = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    double result_execution = get_directly_residuals2_execution(data);
    end = std::chrono::high_resolution_clock::now();
    auto duration_execution = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    double result_simd = get_directly_residuals2_simd(data);
    end = std::chrono::high_resolution_clock::now();
    auto duration_simd = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "ベンチマーク結果:\n";
    std::cout << "基本実装: " << duration_basic << " マイクロ秒 (結果: " << result_basic << ")\n";
    std::cout << "OpenMP実装: " << duration_openmp << " マイクロ秒 (結果: " << result_openmp << ")\n";
    std::cout << "std::execution実装: " << duration_execution << " マイクロ秒 (結果: " << result_execution << ")\n";
    std::cout << "SIMD実装: " << duration_simd << " マイクロ秒 (結果: " << result_simd << ")\n";

    const double epsilon = 1e-10;
    bool results_match = 
        std::abs(result_basic - result_openmp) < epsilon &&
        std::abs(result_basic - result_execution) < epsilon &&
        std::abs(result_basic - result_simd) < epsilon;

    std::cout << "\n結果の一致: " << (results_match ? "すべての実装が同じ結果を返しています" : "警告：実装間で結果が異なります") << "\n";

    std::cout << "\n速度比較（基本実装を1とした場合）:\n";
    double base_time = static_cast<double>(duration_basic);
    std::cout << "OpenMP実装: " << base_time / duration_openmp << "倍\n";
    std::cout << "std::execution実装: " << base_time / duration_execution << "倍\n";
    std::cout << "SIMD実装: " << base_time / duration_simd << "倍\n";
}

int main() {
    std::cout << "パワーロー分布に従うデータを生成中...\n";
    
    std::vector<int> test_sizes = {10000000, 100000000, 200000000};
    
    for (int size : test_sizes) {
        std::cout << "\n=== テストケース: " << size << "点 ===\n";
        std::vector<double> test_data = generate_power_law_point_process(1.5, 1.0, size);
        benchmark_residuals(test_data);
    }
    
    return 0;
}
