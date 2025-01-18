//alpha1.0~2.0に全てを懸けて、F_allも吐き出すプログラム

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include "power_law_tool_equ.hpp"

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
    int t_first_l = 16;
    int t_last_l = 100000;
    
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

    for (double alpha = 1.1 ; alpha <= 2.8; alpha += 0.4) {
        vector<double> slopes;
        vector<double> intercepts;
        vector<vector<double>> all_F_values;
        vector<int> l_values;

        // alphaを少数第1位で表示するために、snprintfを使用
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "%.1f", alpha);
        string alpha_str = buffer;
        string avg_F_values_file = "avg_F/" + alpha_str + ".csv";
        ofstream avg_F_values_output(avg_F_values_file);

        detailed_output << fixed << setprecision(1) << alpha;
        detailed_intercept_output << fixed << setprecision(1) << alpha;


        for (int i = 0; i < number_i; ++i) {
            string directory_name = "F_all/alpha-" + alpha_str;
            string F_all_file_name = directory_name + "/" + to_string(i) + ".csv";
            // ディレクトリが存在しない場合は作成
            string command = "mkdir -p " + directory_name;
            system(command.c_str());
            ofstream F_all_output(F_all_file_name);
            

            std::vector<unsigned int> walk = generate_power_law_point_process(alpha, tau_0, sample_amount);
            std::tuple<double, double, std::vector<int>, std::vector<double>> result = dfa(walk, alpha, t_first_l, t_last_l);
            double slope = std::get<0>(result);
            double intercept = std::get<1>(result);
            std::vector<int> current_l_values = std::get<2>(result);
            std::vector<double> current_F_values = std::get<3>(result);

            if (!std::isnan(slope) && !std::isnan(intercept)) {
                slopes.push_back(slope);
                intercepts.push_back(intercept);
                all_F_values.push_back(current_F_values);
                
                if (i == 0) {  // 最初のイテレーションでl_valuesを保存
                    l_values = current_l_values;
                }

                detailed_output << "," << setprecision(15) << slope;
                detailed_intercept_output << "," << setprecision(15) << intercept;
            } else {
                detailed_output << ",NaN";
                detailed_intercept_output << ",NaN";
                cout << "警告: NaNのslopeまたはinterceptが検出されました。" << endl;
            }
            for (size_t j = 0; j < current_l_values.size(); ++j) {
                F_all_output << current_l_values[j] << "," << setprecision(15) << current_F_values[j] << endl;
            }
            F_all_output.close();
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

        if (!all_F_values.empty()) {
            vector<double> avg_F_values(all_F_values[0].size(), 0.0);
            for (const auto& F_values : all_F_values) {
                for (size_t j = 0; j < F_values.size(); ++j) {
                    avg_F_values[j] += F_values[j];
                }
            }
            for (size_t i = 0; i < avg_F_values.size(); ++i) {
                avg_F_values[i] = avg_F_values[i] / all_F_values.size();
            }

            // 平均F_valuesをCSVに出力
            for (size_t j = 0; j < l_values.size(); ++j) {
                avg_F_values_output << l_values[j] << "," << setprecision(15) << avg_F_values[j] << endl;
            }
        }

    }

    output.close();
    detailed_output.close();
    intercept_output.close();
    detailed_intercept_output.close();
    cout << "結果をファイル " << output_file << " と " << intercept_output_file << " に保存しました。" << endl;
    cout << "詳細な結果をファイル " << detailed_output_file << " と " << detailed_intercept_output_file << " に保存しました。" << endl;
    cout << "すべてのalpha値に対する平均F値の結果を avg_F ディレクトリに保存しました。" << endl;

}