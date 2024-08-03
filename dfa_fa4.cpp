//実行時間計測用

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include "power_law_tool.hpp"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    // デフォルト値の設定
    string output_file = "alpha_slope.csv";
    string detailed_output_file = "alpha_all_slopes.csv";
    double tau_0 = 1.0;
    int sample_amount = pow(10,6);
    int number_i = 10;
    int t_first_l = 16;
    int t_last_l = 10000;

    // コマンドライン引数の解析
    for (int i = 1; i < argc; i += 2) {
        string arg = argv[i];
        if (i + 1 < argc) {
            if (arg == "--output") output_file = argv[i+1];
            else if (arg == "--detailed_output") detailed_output_file = argv[i+1];
            else if (arg == "--tau_0") tau_0 = stod(argv[i+1]);
            else if (arg == "--sample_amount") sample_amount = stoi(argv[i+1]);
            else if (arg == "--number_i") number_i = stoi(argv[i+1]);
            else if (arg == "--t_first_l") t_first_l = stoi(argv[i+1]);
            else if (arg == "--t_last_l") t_last_l = stoi(argv[i+1]);
        }
    }

    cout << "Config values:" << endl;
    cout << "output_file = " << output_file << endl;
    cout << "detailed_output_file = " << detailed_output_file << endl;
    cout << "tau_0 = " << tau_0 << endl;
    cout << "sample_amount = " << sample_amount << endl;
    cout << "number_i = " << number_i << endl;
    cout << "t_first_l = " << t_first_l << endl;
    cout << "t_last_l = " << t_last_l << endl;

    ofstream output(output_file);
    ofstream detailed_output(detailed_output_file);

    for (double alpha = 0.1; alpha <= 3.5; alpha += 0.1) {
        vector<double> slopes;

        detailed_output << fixed << setprecision(1) << alpha;

        for (int i = 0; i < number_i; ++i) {
            auto start_generate = high_resolution_clock::now();
            std::vector<double> walk = generate_power_law_point_process(alpha, tau_0, sample_amount);
            auto stop_generate = high_resolution_clock::now();
            auto duration_generate = duration_cast<seconds>(stop_generate - start_generate);

            auto start_dfa = high_resolution_clock::now();
            double slope = dfa(walk, alpha, t_first_l, t_last_l); 
            auto stop_dfa = high_resolution_clock::now();
            auto duration_dfa = duration_cast<seconds>(stop_dfa - start_dfa);

            cout << "generate_power_law_point_process 実行時間: " << scientific << setprecision(2) << duration_generate.count() << " 秒" << endl;
            cout << "dfa 実行時間: " << scientific << setprecision(2) << duration_dfa.count() << " 秒" << endl;

            if (!std::isnan(slope)) {
                slopes.push_back(slope);
                detailed_output << "," << setprecision(15) << slope;
            } else {
                detailed_output << ",NaN";
                cout << "警告: NaNのslopeが検出されました。" << endl;
            }
        }
        detailed_output << endl;

        if (!slopes.empty()) {
            double average_slope = accumulate(slopes.begin(), slopes.end(), 0.0) / slopes.size();
            output << fixed << setprecision(1) << alpha << "," << setprecision(15) << average_slope << endl;
            cout << "処理済み alpha = " << fixed << setprecision(1) << alpha << endl;
            cout << "平均 slope = " << setprecision(15) << average_slope << " (有効なslope数: " << slopes.size() << ")" << endl;
        } else {
            cout << "警告: alpha = " << fixed << setprecision(1) << alpha << " で有効なslopeがありませんでした。" << endl;
        }
    }
    output.close();
    detailed_output.close();
    cout << "結果をファイル " << output_file << " に保存しました。" << endl;
    cout << "詳細な結果をファイル " << detailed_output_file << " に保存しました。" << endl;
}