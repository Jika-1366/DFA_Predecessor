#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "../utils/power_law_util.hpp"

using namespace std;

// 複数のイベント時刻列を結合してソートする関数
vector<double> merge_multiple_event_times(const vector<vector<double>>& all_times) {
    vector<double> merged;
    // 全体のサイズを予約
    size_t total_size = 0;
    for (const auto& times : all_times) {
        total_size += times.size();
    }
    merged.reserve(total_size);

    // すべての時刻列を結合
    for (const auto& times : all_times) {
        merged.insert(merged.end(), times.begin(), times.end());
    }

    // ソート
    sort(merged.begin(), merged.end());
    return merged;
}
// 隣接する時刻の差分を計算する関数
vector<double> calculate_intervals(const vector<double>& times) {
    vector<double> intervals;
    if (times.size() < 2) return intervals;
    intervals.reserve(times.size() - 1);
    for (size_t i = 0; i < times.size() - 1; ++i) {
        intervals.push_back(times[i+1] - times[i]);
    }
    return intervals;
}

// インターバルをシャッフルしてイベント時刻を再構築する関数
vector<double> shuffle_intervals(const vector<double>& times) {
    if (times.size() < 2) return times;
    
    vector<double> intervals = calculate_intervals(times);
    
    // ランダムデバイスでシードを初期化
    random_device rd;
    mt19937 g(rd());
    shuffle(intervals.begin(), intervals.end(), g);
    
    // 新しいイベント時刻を構築
    vector<double> new_times;
    new_times.reserve(times.size());
    
    double current_time = 0.0;
    for (double dt : intervals) {
        current_time += dt;
        new_times.push_back(current_time);
    }
    
    return new_times;
}


int main() {
    // シミュレーションパラメータ
    const double tau_0 = 1.0;
    const double alpha = 1.5;  // パワーロー分布のスケーリング指数
    const int sample_amount = pow(10, 7);  // サンプル数
    const double T = sample_amount * tau_0;  // 時間範囲
    const int num_sequences = 100;  // 重ね合わせる数
    
    // DFAパラメータ
    const int t_first_l = 16;        // DFA解析の最小スケール
    const int t_last_l = pow(10, 6);       // DFA解析の最大スケール

    // 複数のイベント時刻列を生成
    vector<vector<double>> all_times;
    all_times.reserve(num_sequences);
    
    size_t total_elements = 0;
    
    for (int i = 0; i < num_sequences; ++i) {
        cout << "Processing sequence " << i << endl;
        auto events = simulate_event_times(tau_0, alpha, T);
        total_elements += events.size();
        cout << "Sequence " << i << " size: " << events.size() 
             << " elements, Total elements so far: " << total_elements 
             << " (Theoretical memory: " << (total_elements * sizeof(double)) / (1024.0 * 1024.0) << " MB)" << endl;
        
        all_times.push_back(std::move(events));
    }

    cout << "All sequences generated. Starting merge..." << endl;
    // すべての時刻列を結合してソート
    auto merged_times = merge_multiple_event_times(all_times);
    cout << "Merge completed. Merged size: " << merged_times.size() 
         << " elements (" << (merged_times.size() * sizeof(double)) / (1024.0 * 1024.0) << " MB)" << endl;
    
    // all_timesのメモリを即座に解放
    all_times.clear();
    all_times.shrink_to_fit();
    cout << "Original sequences cleared." << endl;
    
    // インターバルをシャッフルして再構築
    cout << "Starting shuffle..." << endl;
    auto shuffled_times = shuffle_intervals(merged_times);
    cout << "Shuffle completed. Size: " << shuffled_times.size() 
         << " elements (" << (shuffled_times.size() * sizeof(double)) / (1024.0 * 1024.0) << " MB)" << endl;
    
    // merged_timesのメモリを即座に解放
    merged_times.clear();
    merged_times.shrink_to_fit();
    cout << "Merged times cleared." << endl;

    // 累積カウントを計算（sample_amountまでの時間で固定）
    auto counts = count_events_per_unit_time(shuffled_times, sample_amount);
    
    // DFA解析を実行して結果を返す
    auto [slope, intercept, l_values, F_values] = dfa_F2(counts, alpha, t_first_l, t_last_l);
    
    cout << slope << endl;
    
    return 0;
} 