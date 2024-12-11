#pragma once

#include <vector>
#include <numeric>
#include <execution>
#include <chrono>
#include <omp.h>
#include <immintrin.h>
#include <cmath>

// Legacy手法：直線フィッティングによる残差計算
double get_residuals_legacy(const std::vector<double>& y) {
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

    return residual_sum_squares / (n);  // 残差の平均を返すように修正
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
    long double second_term = (12.0*(n+1)/(n-1))*std::pow((weighted_sum/n/(n+1) - mean/2),2);

    return v2_y - second_term;
}

// OpenMP並列処理版
double get_residuals_openmp(const std::vector<double>& y) {
    int n = y.size();
    if (n == 0) {
        return 0.0;
    }

    if (n < 10000) {
        return get_residuals_direct(y);
    }

    double sum = 0.0;
    #pragma omp parallel reduction(+:sum)
    {
        #pragma omp for
        for (int i = 0; i < n; ++i) {
            sum += y[i];
        }
    }
    double mean = sum / n;

    long double sq_sum = 0.0L;
    long double weighted_sum = 0.0L;

    #pragma omp parallel reduction(+:sq_sum,weighted_sum)
    {
        #pragma omp for
        for (int i = 0; i < n; ++i) {
            double value = y[i];
            sq_sum += value * value;
            weighted_sum += (i+1) * value;
        }
    }

    long double v2_y = (sq_sum / n) - mean*mean;
    long double second_term = (12.0*(n+1)/(n-1))*std::pow((weighted_sum/n/(n+1) - mean/2),2);

    return v2_y - second_term;
}

// SIMD並列処理版
double get_residuals_simd(const std::vector<double>& y) {
    int n = y.size();
    if (n == 0) {
        return 0.0;
    }

    if (n < 1000 || n % 4 != 0) {
        return get_residuals_direct(y);
    }

    // 平均値の計算
    __m256d sum_vec = _mm256_setzero_pd();
    for (int i = 0; i < n; i += 4) {
        __m256d y_vec = _mm256_loadu_pd(&y[i]);
        sum_vec = _mm256_add_pd(sum_vec, y_vec);
    }
    double sum_arr[4];
    _mm256_storeu_pd(sum_arr, sum_vec);
    double sum = sum_arr[0] + sum_arr[1] + sum_arr[2] + sum_arr[3];
    double mean = sum / n;

    // sq_sumとweighted_sumの計算
    __m256d sq_sum_vec = _mm256_setzero_pd();
    __m256d weighted_sum_vec = _mm256_setzero_pd();
    
    for (int i = 0; i < n; i += 4) {
        __m256d y_vec = _mm256_loadu_pd(&y[i]);
        // インデックスを1から始めるように修正
        __m256d idx_vec = _mm256_set_pd(i+4+1, i+3+1, i+2+1, i+1+1);
        
        // sq_sum計算
        sq_sum_vec = _mm256_add_pd(sq_sum_vec, _mm256_mul_pd(y_vec, y_vec));
        
        // weighted_sum計算
        weighted_sum_vec = _mm256_add_pd(weighted_sum_vec, _mm256_mul_pd(idx_vec, y_vec));
    }

    double sq_sum_arr[4], weighted_sum_arr[4];
    _mm256_storeu_pd(sq_sum_arr, sq_sum_vec);
    _mm256_storeu_pd(weighted_sum_arr, weighted_sum_vec);

    long double sq_sum = sq_sum_arr[0] + sq_sum_arr[1] + sq_sum_arr[2] + sq_sum_arr[3];
    long double weighted_sum = weighted_sum_arr[0] + weighted_sum_arr[1] + weighted_sum_arr[2] + weighted_sum_arr[3];

    long double v2_y = (sq_sum / n) - mean*mean;
    long double second_term = (12.0*(n+1)/(n-1))*std::pow((weighted_sum/n/(n+1) - mean/2),2);

    return v2_y - second_term;
}

// Power Law Tool 7の実装を使用した手法
double get_residuals_power_law_7(const std::vector<double>& y) {
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
    long double second_term = (12.0*(n+1)/(n-1))*std::pow((weighted_sum/n/(n+1) - mean/2),2);

    return v2_y - second_term;
}
