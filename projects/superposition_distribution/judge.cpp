#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "../utils/power_law_util.hpp"

using namespace std;

// イベント時刻列を累積カウントに変換して加算する関数
void add_sequence_to_counts(vector<unsigned int>& total_counts, const vector<double>& times, int sample_amount) {
    vector<unsigned int> sequence_counts = count_events_per_unit_time(times, sample_amount);
    for (size_t i = 0; i < total_counts.size(); ++i) {
        total_counts[i] += sequence_counts[i];
    }
}

int main() {
    // シミュレーションパラメータ
    const double tau_0 = 1.0;
    const double alpha = 1.5;  // パワーロー分布のスケーリング指数
    const int sample_amount = pow(10, 6);  // サンプル数
    const double T = sample_amount * tau_0;  // 時間範囲
    const int num_sequences = 100;  // 重ね合わせる数
    
    // DFAパラメータ
    const int t_first_l = 16;        // DFA解析の最小スケール
    const int t_last_l = pow(10, 5);       // DFA解析の最大スケール

    // 累積カウントを保持するベクター
    vector<unsigned int> total_counts(sample_amount, 0);
    
    // 1つずつシーケンスを生成して処理
    for (int i = 0; i < num_sequences; ++i) {
        cout << "Processing sequence " << i << endl;
        auto times = simulate_event_times(tau_0, alpha, T);
        add_sequence_to_counts(total_counts, times, sample_amount);
        // timesはここでスコープを抜けて自動的に解放される
    }
    
    // DFA解析を実行して結果を返す
    auto [slope, intercept, l_values, F_values] = dfa_F2(total_counts, alpha, t_first_l, t_last_l);
    
    cout << slope << endl;
    
    return 0;
} 