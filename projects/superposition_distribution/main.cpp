//alpha1.0~2.0に全てを懸けて、F_allも吐き出すプログラム

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include "../utils/power_law_util.hpp"

using namespace std;



vector<unsigned int> super_position(vector<unsigned int> walk_1, vector<unsigned int> walk_2) {
    vector<unsigned int> walk_super;
    for (size_t i = 0; i < walk_1.size(); ++i) {
        walk_super.push_back(walk_1[i]+walk_2[i]);
    }
    return walk_super;
}


int main() {
    double tau_0 = 1.0;
    int sample_amount = pow(10,8);
    int t_first_l = 16;
    int t_last_l = pow(10,7);



    const int num_trials = 1;
    vector<vector<double>> results1_1(num_trials);
    vector<vector<double>> results1_5(num_trials); 
    vector<vector<double>> results1_5_and_1_1(num_trials);
    vector<int> time_scales; // 時間スケールを保存

    for (int trial = 0; trial < num_trials; trial++) {
        vector<unsigned int> walk_1_1 = generate_power_law_point_process(1.1, tau_0, sample_amount);
        auto result1_1 = dfa_F2(walk_1_1, 1.1, t_first_l, t_last_l);
        results1_1[trial] = std::get<3>(result1_1);

        vector<unsigned int> walk_1_5 = generate_power_law_point_process(1.5, tau_0, sample_amount);
        auto result1_5 = dfa_F2(walk_1_5, 1.5, t_first_l, t_last_l);
        results1_5[trial] = std::get<3>(result1_5);

        vector<unsigned int> walk_1_5_and_1_1 = super_position(walk_1_5, walk_1_1);
        auto result1_5_and_1_1 = dfa_F2(walk_1_5_and_1_1, 1000.0, t_first_l, t_last_l);
        results1_5_and_1_1[trial] = std::get<3>(result1_5_and_1_1);

        if (trial == 0) {
            time_scales = std::get<2>(result1_1);
        }
    }

    // 各時間スケールごとに平均を計算
    for (size_t i = 0; i < results1_1[0].size(); ++i) {
        double avg1_1 = 0, avg1_5 = 0, avg1_5_and_1_1 = 0;
        
        for (int trial = 0; trial < num_trials; trial++) {
            avg1_1 += results1_1[trial][i];
            avg1_5 += results1_5[trial][i];
            avg1_5_and_1_1 += results1_5_and_1_1[trial][i];
        }
        
        avg1_1 /= num_trials;
        avg1_5 /= num_trials;
        avg1_5_and_1_1 /= num_trials;

        cout << time_scales[i] << ",";
        cout << avg1_1 << ",";
        cout << avg1_5 << ",";
        cout << "sum_of_left, ";
        cout << avg1_1 + avg1_5;
        cout << "             target";
        cout << avg1_5_and_1_1 << endl;
    }
    
}

