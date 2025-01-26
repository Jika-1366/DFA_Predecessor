#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "../utils/power_law_util.hpp"

using namespace std;

// 二重和を計算する関数（long double を使用）
long double calculate_double_sum_3rd_term(const vector<unsigned int>& y) {
    int n = y.size();
    double sum = 0.0;
    double prefix_sum = 0.0;       // y[0] + y[1] + ... + y[i-1] の累積和
    double prefix_sum_squared = 0.0; // y[0]^2 + y[1]^2 + ... + y[i-1]^2 の累積和

    for (int i = 0; i < n; ++i) {
        // sum += y[i] * (prefix_sum) - prefix_sum_squared
        sum += y[i] * prefix_sum - prefix_sum_squared;

        // 累積和を更新
        prefix_sum += y[i];
        prefix_sum_squared += y[i] * y[i];
    }

    return 2.0*sum;
}

// 二重和を計算する関数（long double を使用）
long double calculate_double_sum_2nd_term(const vector<unsigned int>& y) {
    int n = y.size();
    long double sum = 0.0L;

    long double c = (n + 1.0L) / 2.0L;  // 中心値

    // Σ(i - c)y[i] を計算
    long double sum_centered = 0.0L;
    for (int i = 0; i < n; ++i) {
        long double term = (i + 1 - c) * y[i]; // i+1は1-basedインデックスに対応
        sum_centered += term;
    }

    sum = sum_centered * sum_centered;
    // 理論的には第2項はゼロになるため無視

    return 2.0L*sum;
}





int main() {
    double tau_0 = 1.0;
    int sample_amount = pow(10, 6);  // サンプル数を10^7に設定
    double T = sample_amount * tau_0;
    double alpha = 1.5;  // 固定のalpha値

    double total_sum_3rd = 0.0;
    double total_sum_2nd = 0.0;

    // 100回の繰り返し
    for (int iter = 0; iter < 100; ++iter) {
        // イベント時刻をシミュレーション
        auto times = simulate_event_times(tau_0, alpha, T);
        
        // 単位時間あたりのイベント数をカウント
        auto counts = count_events_per_unit_time(times, sample_amount);
        
        // 二重和を計算
        double double_sum_3rd = calculate_double_sum_3rd_term(counts);
        double double_sum_2nd = calculate_double_sum_2nd_term(counts);
        
        double a_double_sum_3rd = double_sum_3rd / pow(sample_amount, 2);
        double a_double_sum_2nd = 12*double_sum_2nd / pow(sample_amount, 4);

        total_sum_3rd += a_double_sum_3rd;
        total_sum_2nd += a_double_sum_2nd;
        

        cout << "Iteration " << iter + 1 << " completed. Double sum: " << a_double_sum_3rd << endl;
        cout << "Iteration " << iter + 1 << " completed. Double sum: " << a_double_sum_2nd << endl;
    }

    cout << "Total sum 3rd: " << total_sum_3rd/100 << endl;
    cout << "Total sum 2nd: " << total_sum_2nd/100 << endl;

    return 0;
}