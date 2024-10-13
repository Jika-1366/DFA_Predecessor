#ifndef POWER_LAW_TOOL_INT_HPP
#define POWER_LAW_TOOL_INT_HPP
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

std::vector<double> generate_power_law_walk(double alpha, double tau_0 = 1.0, unsigned int sample_amount = 100000);

std::vector<unsigned int> generate_power_law_renewal_process(double alpha,
                                                    double tau_0 = 1.0,
                                                    unsigned int sample_amount = 100000
                                                    );

std::tuple<double, double, std::vector<unsigned int>, std::vector<double>> dfa(std::vector<unsigned int> RW_list, double alpha = 0.0, unsigned int first_l = 4, unsigned int t_last_l= 10000);

#endif