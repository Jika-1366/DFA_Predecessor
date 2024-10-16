#ifndef POWER_LAW_TOOL_HPP_7
#define POWER_LAW_TOOL_HPP_7
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

std::pair<std::vector<double>, double> generate_power_law_point_process(double alpha,
                                                    double tau_0 = 1.0,
                                                    int sample_amount = 100000,
                                                    double exceeded_waiting_time_parameter = 0.0
                                                    );
double get_sum_squared_residuals(std::vector<double> walk);

std::vector<std::pair<int, double>> calculate_F_values(double alpha, 
                                    double tau_0, int number_of_segments, 
                                    int first_i, int last_i);

std::tuple<double, double, double> find_best_fit(const std::vector<double>& y, const std::vector<double>& x = std::vector<double>());

std::tuple<double, double, std::vector<int>, std::vector<double>> dfa(std::vector<double> RW_list, double alpha = 0.0, int first_l = 4, int t_last_l= 10000);

#endif