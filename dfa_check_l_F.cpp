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
    double tau_0 = 1.0;
    int sample_amount = pow(10,8);
    int number_i = 1;
    int t_first_l = 16;
    int t_last_l = pow(10,6);
    long long cap_rate = 1;

    // コマンドライン引数の解析
    for (int i = 1; i < argc; i += 2) {
        string arg = argv[i];
        if (i + 1 < argc) {
            if (arg == "--tau_0") tau_0 = stod(argv[i+1]);
            else if (arg == "--sample_amount") sample_amount = stoi(argv[i+1]);
            else if (arg == "--number_i") number_i = stoi(argv[i+1]);
            else if (arg == "--t_first_l") t_first_l = stoi(argv[i+1]);
            else if (arg == "--t_last_l") t_last_l = stoi(argv[i+1]);
            else if (arg == "--cap_rate") cap_rate = stoll(argv[i+1]);
        }
    }

    cout << "Config values:" << endl;
    cout << "tau_0 = " << tau_0 << endl;
    cout << "sample_amount = " << sample_amount << endl;
    cout << "number_i = " << number_i << endl;
    cout << "t_first_l = " << t_first_l << endl;
    cout << "t_last_l = " << t_last_l << endl;
    cout << "cap_rate = " << cap_rate << endl;

    for (double alpha = 0.1; alpha <= 3.5; alpha += 0.1) {
        vector<double> slopes;

        for (int i = 0; i < number_i; ++i) {
            std::vector<double> walk = generate_power_law_point_process(alpha, tau_0, sample_amount, cap_rate);
            double slope = dfa(walk, alpha, t_first_l, t_last_l);
            if (!std::isnan(slope)) {
                slopes.push_back(slope);
            } else {
                cout << "警告: NaNのslopeが検出されました。" << endl;
            }
        }

        if (!slopes.empty()) {
            double average_slope = accumulate(slopes.begin(), slopes.end(), 0.0) / slopes.size();
            cout << "処理済み alpha = " << fixed << setprecision(1) << alpha << endl;
            cout << "平均 slope = " << setprecision(15) << average_slope << " (有効なslope数: " << slopes.size() << ")" << endl;
        } else {
            cout << "警告: alpha = " << fixed << setprecision(1) << alpha << " で有効なslopeがありませんでした。" << endl;
        }
    }
}