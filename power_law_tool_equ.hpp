#ifndef POWER_LAW_TOOL_EQU_HPP
#define POWER_LAW_TOOL_EQU_HPP
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <cmath>
#include <limits>
#include <ctime>
#include <algorithm>
#include <tuple>


std::vector<unsigned long long> generate_power_law_point_process(double alpha,
                                                    double tau_0 = 1.0,
                                                    int sample_amount = 100000
                                                    );

std::tuple<double, double, std::vector<int>, std::vector<double>> dfa(std::vector<unsigned long long> RW_list, double alpha = 0.0, int first_l = 4, int t_last_l= 10000);

#endif