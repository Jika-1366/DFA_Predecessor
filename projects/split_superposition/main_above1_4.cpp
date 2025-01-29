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

// 2つのイベント時刻列を結合してソートする関数
vector<double> merge_event_times(const vector<double>& times1, const vector<double>& times2) {
    vector<double> merged;
    merged.reserve(times1.size() + times2.size());
    merged.insert(merged.end(), times1.begin(), times1.end());
    merged.insert(merged.end(), times2.begin(), times2.end());
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
    double tau_0 = 1.0;
    //int sample_amount = pow(10, 8);  // サンプル数を調整可能
    //int t_first_l = 2e4;
    //int t_last_l = 6e5;
    int sample_amount = pow(10, 7);  // サンプル数を調整可能
    int t_first_l = 1e4;
    int t_last_l = 1e5;
    
    int number_i = 10;

    double T = sample_amount * tau_0;  // 十分な時間範囲

    // 出力ディレクトリの設定
    string output_dir = "avg_F2/";

    // alpha値の範囲を設定（0.3刻み）
    vector<double> alphas;
    for (double alpha = 1.4; alpha <= 2.0; alpha += 0.1) {
        alphas.push_back(round(alpha * 10) / 10);  // 浮動小数点の誤差を防ぐ
    }
    // すべての組み合わせについて処理
    for (size_t i = 0; i < alphas.size(); ++i) {
        for (size_t j = i; j < alphas.size(); ++j) {
            double alpha1 = alphas[i];
            double alpha2 = alphas[j];

            // 10回の試行のためのベクトル
            vector<vector<double>> F_values_trials;
            vector<double> l_values;

            // 10回試行
            for (int trial = 0; trial < number_i; ++trial) {
                // イベント時刻をシミュレーション
                auto times1 = simulate_event_times(tau_0, alpha1, T);
                auto times2 = simulate_event_times(tau_0, alpha2, T);

                // イベント時刻を結合してソート
                auto merged_times = merge_event_times(times1, times2);

                // 累積カウントを計算（sample_amountまでの時間で固定）
                auto counts = count_events_per_unit_time(merged_times, sample_amount);

                // DFA解析を実行して結果を返す
                auto [slope, intercept, l_vals, F_vals] = dfa_F2(counts, alpha1, t_first_l, t_last_l);

                // F値を保存
                F_values_trials.push_back(F_vals);

                //l_valuesが定義されていなくて、返ってきたcurrent_l_valuesが0でない場合は、l_valuesをcurrent_l_valuesにする。これを使う。
                if (l_values.empty() && !l_vals.empty() && std::any_of(l_vals.begin(), l_vals.end(), [](int x) { return x != 0; })) {
                    l_values.assign(l_vals.begin(), l_vals.end());
                }
            }

            // F値の平均を計算
            vector<double> avg_F_values(l_values.size(), 0.0);
            for (size_t k = 0; k < l_values.size(); ++k) {
                double sum = 0.0;
                for (const auto& F_vals : F_values_trials) {
                    sum += F_vals[k];
                }
                avg_F_values[k] = sum / number_i;
            }

            // ファイル名を生成
            ostringstream filename;
            filename << output_dir << "avg_dfa_alpha" << fixed << setprecision(1) 
                    << alpha1 << "_" << alpha2 << ".csv";

            // 平均結果をファイルに出力
            ofstream outfile(filename.str());
            for (size_t k = 0; k < l_values.size(); ++k) {
                outfile << l_values[k] << "," << avg_F_values[k] << "\n";
            }
            outfile.close();

            cout << "Processed alpha1 = " << alpha1 
                 << ", alpha2 = " << alpha2 
                 << ", averaged over " << number_i << " trials and saved to " << filename.str() << endl;
        }
    }

    return 0;
}



