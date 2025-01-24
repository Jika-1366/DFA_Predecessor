#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include "../utils/power_law_util.hpp"

using namespace std;

// 指数分布の待ち時間を生成する関数
vector<double> simulate_exponential_times(double lambda, double T) {
    vector<double> event_times;
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<double> exp_dist(lambda);
    
    double current_time = 0.0;
    while (current_time < T) {
        double waiting_time = exp_dist(gen);
        current_time += waiting_time;
        if (current_time >= T) break;
        event_times.push_back(current_time);
    }
    
    return event_times;
}

int main() {
    // シミュレーションパラメータ
    const double lambda = 22.91;  // 指数分布のレート
    const int sample_amount = pow(10, 8);  // サンプル数
    const double T = sample_amount;  // 時間範囲
    
    // DFAパラメータ
    const int t_first_l = 16;
    const int t_last_l = pow(10, 7);

    // イベント時刻列を生成
    auto event_times = simulate_exponential_times(lambda, T);
    
    // 累積カウントを計算
    auto counts = count_events_per_unit_time(event_times, sample_amount);
    
    // DFA解析を実行して結果を返す
    auto [slope, intercept, l_values, F_values] = dfa_F2(counts, 0.5, t_first_l, t_last_l);
    
    cout << slope << endl;
    
    return 0;
} 