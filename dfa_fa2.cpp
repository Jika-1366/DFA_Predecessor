#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include "power_law_tool.hpp"

using namespace std;

int main() {
    string output_file = "alpha_slope.csv";
    string detailed_output_file = "alpha_all_slopes.csv";
    double tau_0 = 1.0;
    int sample_amount = pow(10,7);
    int number_i = 30;
    int t_first_l = 16;
    cout << "Config values:" << endl;
    cout << "tau_0 = " << tau_0 << endl;
    cout << "sample_amount = " << sample_amount << endl;
    cout << "number_i = " << number_i << endl;
    cout << "t_first_l = " << t_first_l << endl;

    ofstream output(output_file);
    ofstream detailed_output(detailed_output_file);
    
    // 詳細なCSVファイルのヘッダーを書き込む
    detailed_output << "alpha,slope1,slope2,slope3,slope4,slope5,slope6,slope7,slope8,slope9,slope10" << endl;

    for (double alpha = 0.1; alpha <= 3.5; alpha += 0.1) {
        vector<double> slopes;

        detailed_output << fixed << setprecision(1) << alpha;

        for (int i = 0; i < number_i; ++i) {
            std::vector<double> walk = generate_power_law_point_process(alpha, tau_0, sample_amount);
            double slope = dfa(walk, alpha = alpha, t_first_l);
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