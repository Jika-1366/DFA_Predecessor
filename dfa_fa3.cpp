//使用すべきlの範囲を調べるためのプログラム

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include "power_law_tool.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    // デフォルト値の設定
    string output_file = "alpha_slope.csv";
    string detailed_output_file = "alpha_all_slopes.csv";
    string intercept_output_file = "alpha_intercept.csv";
    string detailed_intercept_output_file = "alpha_all_intercepts.csv";
    double tau_0 = 1.0;
    int sample_amount = pow(10,6);
    int number_i = 20;
    int t_first_l = 10000;
    int t_last_l = 1000000;

    // コマンドライン引数の解析
    for (int i = 1; i < argc; i += 2) {
        string arg = argv[i];
        if (i + 1 < argc) {
            if (arg == "--output") output_file = argv[i+1];
            else if (arg == "--detailed_output") detailed_output_file = argv[i+1];
            else if (arg == "--intercept_output") intercept_output_file = argv[i+1];
            else if (arg == "--detailed_intercept_output") detailed_intercept_output_file = argv[i+1];
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
    cout << "intercept_output_file = " << intercept_output_file << endl;
    cout << "detailed_intercept_output_file = " << detailed_intercept_output_file << endl;
    cout << "tau_0 = " << tau_0 << endl;
    cout << "sample_amount = " << sample_amount << endl;
    cout << "number_i = " << number_i << endl;
    cout << "t_first_l = " << t_first_l << endl;
    cout << "t_last_l = " << t_last_l << endl;

    ofstream output(output_file);
    ofstream detailed_output(detailed_output_file);
    ofstream intercept_output(intercept_output_file);
    ofstream detailed_intercept_output(detailed_intercept_output_file);

    for (double alpha = 0.1; alpha <= 3.5; alpha += 0.1) {
        vector<double> slopes;
        vector<double> intercepts;

        detailed_output << fixed << setprecision(1) << alpha;
        detailed_intercept_output << fixed << setprecision(1) << alpha;

        for (int i = 0; i < number_i; ++i) {
            std::vector<double> walk = generate_power_law_point_process(alpha, tau_0, sample_amount);
            std::pair<double, double> result = dfa(walk, alpha, t_first_l, t_last_l);
            double slope = result.first;
            double intercept = result.second;

            if (!std::isnan(slope) && !std::isnan(intercept)) {
                slopes.push_back(slope);
                intercepts.push_back(intercept);
                detailed_output << "," << setprecision(15) << slope;
                detailed_intercept_output << "," << setprecision(15) << intercept;
            } else {
                detailed_output << ",NaN";
                detailed_intercept_output << ",NaN";
                cout << "警告: NaNのslopeまたはinterceptが検出されました。" << endl;
            }
        }
        detailed_output << endl;
        detailed_intercept_output << endl;

        if (!slopes.empty() && !intercepts.empty()) {
            double average_slope = accumulate(slopes.begin(), slopes.end(), 0.0) / slopes.size();
            double average_intercept = accumulate(intercepts.begin(), intercepts.end(), 0.0) / intercepts.size();
            output << fixed << setprecision(1) << alpha << "," << setprecision(15) << average_slope << endl;
            intercept_output << fixed << setprecision(1) << alpha << "," << setprecision(15) << average_intercept << endl;
            cout << "処理済み alpha = " << fixed << setprecision(1) << alpha << endl;
            cout << "平均 slope = " << setprecision(15) << average_slope << " (有効なslope数: " << slopes.size() << ")" << endl;
            cout << "平均 intercept = " << setprecision(15) << average_intercept << " (有効なintercept数: " << intercepts.size() << ")" << endl;
        } else {
            cout << "警告: alpha = " << fixed << setprecision(1) << alpha << " で有効なslopeまたはinterceptがありませんでした。" << endl;
        }
    }
    output.close();
    detailed_output.close();
    intercept_output.close();
    detailed_intercept_output.close();
    cout << "結果をファイル " << output_file << " と " << intercept_output_file << " に保存しました。" << endl;
    cout << "詳細な結果をファイル " << detailed_output_file << " と " << detailed_intercept_output_file << " に保存しました。" << endl;
}