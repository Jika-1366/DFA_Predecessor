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
    
    // 一様分布から指数分布を生成する
    uniform_real_distribution<double> uniform_dist(0.0, 1.0);
    
    double current_time = 0.0;
    while (current_time < T) {
        // 一様分布から指数分布への変換
        // F(x) = 1 - e^(-λx) の逆関数を使用
        double u = uniform_dist(gen);
        double waiting_time = -log(1.0 - u) / lambda;
        
        current_time += waiting_time;
        if (current_time >= T) break;
        event_times.push_back(current_time);
    }
    
    return event_times;
}

int main() {
    // シミュレーションパラメータ
    const double lambda = 22.91;  // 指数分布のレート
    //const double lambda = 0.3;
    
    const int sample_amount = pow(10, 7);  // サンプル数
    const double T = sample_amount;  // 時間範囲
    
    // DFAパラメータ
    const int t_first_l = 16;
    const int t_last_l = pow(10, 6);
    cout << "lambda: " << lambda << endl;
    cout << "sample_amount: " << sample_amount << endl;
    cout << "t_last_l: " << t_last_l << endl;
    // イベント時刻列を生成
    cout << "Generating event times..." << endl;
    auto event_times = simulate_exponential_times(lambda, T);
    cout << "Event times generated." << endl;
    
    // 累積カウントを計算

    auto counts = count_events_per_unit_time(event_times, sample_amount);
    cout << "Counts calculated." << endl;
    // DFA解析を実行して結果を返す
    auto [slope, intercept, l_values, F_values] = dfa_F2(counts, 0.5, t_first_l, t_last_l);
    
    cout << slope << endl;
    // l値とF値をCSVファイルに保存
    ofstream l_file("l_values.csv");
    ofstream F_file("F_values.csv");
    
    // 各値を書き込み
    for(size_t i = 0; i < l_values.size(); i++) {
        l_file << l_values[i] << "\n";
        F_file << F_values[i] << "\n";
    }
    
    l_file.close();
    F_file.close();
    
    cout << "Data saved to l_values.csv and F_values.csv" << endl;
    return 0;
} 