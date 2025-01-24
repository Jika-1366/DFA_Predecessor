#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>
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
    intervals.reserve(times.size() - 1);
    for (size_t i = 0; i < times.size() - 1; ++i) {
        intervals.push_back(times[i + 1] - times[i]);
    }
    return intervals;
}

int main() {
    const double tau_0 = 1.0;
    const double alpha = 1.5;  // 固定のalpha値
    const int sample_amount = pow(10, 5);  // サンプル数
    const double T = sample_amount * tau_0;  // 時間範囲
    const int num_sequences = 100;  // 重ね合わせる数

    cout << "Generating " << num_sequences << " event time sequences with alpha = " << alpha << "..." << endl;

    // 複数のイベント時刻列を生成
    vector<vector<double>> all_times;
    all_times.reserve(num_sequences);
    
    for (int i = 0; i < num_sequences; ++i) {
        if (i % 10 == 0) {
            cout << "Generated " << i << "/" << num_sequences << " sequences" << endl;
        }
        all_times.push_back(simulate_event_times(tau_0, alpha, T));
    }

    cout << "Merging sequences..." << endl;
    // すべての時刻列を結合してソート
    auto merged_times = merge_multiple_event_times(all_times);

    cout << "Calculating intervals..." << endl;
    // 時間間隔を計算
    auto intervals = calculate_intervals(merged_times);

    // 結果をファイルに出力
    string filename = "superposed_intervals_100.csv";
    cout << "Saving results to " << filename << "..." << endl;
    
    ofstream outfile(filename);
    for (const auto& interval : intervals) {
        outfile << interval << "\n";
    }
    outfile.close();

    cout << "Done!" << endl;
    return 0;
} 