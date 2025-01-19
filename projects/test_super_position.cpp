//alpha1.0~2.0に全てを懸けて、F_allも吐き出すプログラム

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include "utils/power_law_util.hpp"

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
    int t_last_l = 10000000;



    vector<unsigned int> walk_1_1 = generate_power_law_point_process(1.1, tau_0, sample_amount);
    std::tuple<double, double, std::vector<int>, std::vector<double>> result1_1 = dfa_F2(walk_1_1, 1.1, t_first_l, t_last_l);

    vector<unsigned int> walk_1_5 = generate_power_law_point_process(1.5, tau_0, sample_amount);
    std::tuple<double, double, std::vector<int>, std::vector<double>> result1_5 = dfa_F2(walk_1_5, 1.5, t_first_l, t_last_l);

    vector<unsigned int> walk_1_5_and_1_1 = super_position(walk_1_5, walk_1_1);
    std::tuple<double, double, std::vector<int>, std::vector<double>> result1_5_and_1_1 = dfa_F2(walk_1_5_and_1_1, 1.16, t_first_l, t_last_l);

    for (size_t i = 0; i < std::get<3>(result1_1).size(); ++i) {

        cout << std::get<2>(result1_1)[i] << ",";
        cout << std::get<3>(result1_1)[i] << ",";
        cout << std::get<3>(result1_5)[i] << ",";
        cout << "sum_of_left, ";
        cout << std::get<3>(result1_1)[i] + std::get<3>(result1_5)[i];
        cout << "             target";
        cout << std::get<3>(result1_5_and_1_1)[i] << endl;
    }
}

