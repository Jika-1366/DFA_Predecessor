#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include <tuple>
#include <stdexcept>
#include <random>
#include <vector>
#include <cmath>
#include <queue> // 追加: queueを使うために必要

#include "power_law_tool.hpp"

using namespace std;


std::vector<double> generate_power_law_walk(double alpha, double tau_0, int sample_amount) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    std::vector<double> samples(sample_amount);
    
    for (int i = 0; i < sample_amount; ++i) {
        double u = dis(gen);
        // べき分布の逆関数を使用し、tau_0をスケールファクターとして使用
        samples[i] = tau_0 * std::pow(u, -1.0 / alpha);
    }
    
    std::vector<double> walk(sample_amount);
    walk[0] = samples[0];
    for (int i = 1; i < sample_amount; ++i) {
        walk[i] = walk[i-1] + samples[i];
    }
    
    return walk;
}    


std::vector<double> generate_power_law_point_process(double alpha, double tau_0, int sample_amount) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    std::queue<double> samplesQueue; // 変更: vectorからqueueに変更

    for (int i = 0; i < sample_amount; ++i) {
        double u = dis(gen);
        // べき分布の逆関数を使用し、tau_0をスケールファクターとして使用
        //inter_occurence_timeこそがsamplesです。
        samplesQueue.push(tau_0 * std::pow(u, -1.0 / alpha)); // 変更: queueに値を追加
    }
    

    std::vector<double> N(sample_amount);
    N[0] = 0; 
    //tとは時刻のこと。時刻の積み重ねがinter_occurence_timeを越えたら、Nを+1して、積み重ねをリセット
    int peace_time = 0;
    for (int t = 1; t < sample_amount; ++t) {
        peace_time += 1;
        if (peace_time >= samplesQueue.front()){ // 変更: queueの先頭から値を取得
            peace_time = 0;
            N[t]= N[t-1] +1; 
            samplesQueue.pop(); // queueの先頭の値をqueueから削除
            }
            
        else{N[t] = N[t-1];
        }

    }
    
    return N;
}    



// 傾き、切片、残差の二乗の合計を返す。yは必須だが、xは渡しても渡さなくともいい。
std::tuple<double, double, double> find_best_fit(const std::vector<double>& y, const std::vector<double>& x = std::vector<double>()) {
    int n = y.size();
    std::vector<double> x_values;

    // xが空の時だけ、適当に作成。
    if (x.empty()) {
        x_values.reserve(n);
        for (int i = 0; i < n; ++i) {
            x_values.push_back(i + 1.0);  // x = 1, 2, ..., n
        }
    } else if (x.size() != n) {
        throw std::invalid_argument("x and y must have the same size");
    } else {
        x_values = x;  // 既存のxをコピー
    }

    // 平均を計算
    double x_mean = 0.0, y_mean = 0.0;
    for (int i = 0; i < n; ++i) {
        x_mean += x_values[i];
        y_mean += y[i];
    }
    x_mean /= n;
    y_mean /= n;

    // 傾きと切片を計算
    double numerator = 0.0, denominator = 0.0;
    for (int i = 0; i < n; ++i) {
        numerator += (x_values[i] - x_mean) * (y[i] - y_mean);
        denominator += (x_values[i] - x_mean) * (x_values[i] - x_mean);
    }

    double slope = numerator / denominator;
    double intercept = y_mean - slope * x_mean;

    // 残差の二乗の合計を計算
    double residual_sum_squares = 0.0;
    for (int i = 0; i < n; ++i) {
        double residual = y[i] - (slope * x_values[i] + intercept);
        residual_sum_squares += residual * residual;
    }

    return std::make_tuple(slope, intercept, residual_sum_squares);
}

// Find the parameters that minimize the residual.
tuple<double, double, double> old_find_best_fit(const vector<double>& y) {
    int n = y.size();
    vector<int> x(n);
    iota(x.begin(), x.end(), 1);  // x = 1, 2, ..., n

    double best_slope = 0;
    double best_intercept = 0;
    double best_error = numeric_limits<double>::infinity();

    // Search range and step size
    const double slope_range = 10.0;
    const double intercept_range = 1000.0;
    const double slope_step = 0.01;
    const double intercept_step = 0.1;
    for (double slope = -slope_range; slope <= slope_range; slope += slope_step) {
        for (double intercept = -intercept_range; intercept <= intercept_range; intercept += intercept_step) {
            double error = 0;

            for (int i = 0; i < n; ++i) {
                double predicted = slope * x[i] + intercept;
                double residual = y[i] - predicted;
                error += residual * residual;
            }

            if (error < best_error) {
                best_error = error;
                best_slope = slope;
                best_intercept = intercept;
            }
        }
    }

    return make_tuple(best_slope, best_intercept, best_error);
}

// Function to divide data
vector<vector<double>> generate_segments(const vector<double>& data, int scale) {
    vector<vector<double>> segments;
    int num_segments = data.size() / scale;
    for (int i = 0; i < num_segments; ++i) {
        vector<double> segment(data.begin() + i * scale, data.begin() + ((i + 1) * scale));
        segments.push_back(segment);
    }
    return segments;
}

// Function to divide data
vector<vector<double>> generate_segments_2(const vector<double>& data, int scale) {
    vector<vector<double>> segments;
    int num_segments = data.size() / scale;
    for (int i = 0; i < num_segments; ++i) {
        vector<double> segment(data.begin() + i * scale, data.begin() + (i + 1) * scale);
        segments.push_back(segment);
    }
    // Handling when not divisible
    if (data.size() % scale != 0) {
        vector<double> segment(data.begin() + num_segments * scale, data.end());
        segments.push_back(segment);
    }
    return segments;
}



double dfa(vector<double> RW_list, double alpha, int first_l) {
     // alphaを少数第1位で表示するために、snprintfを使用します。
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%.1f", alpha);
    string base_name = buffer;
    
    //string input_path = base_name + ".csv";
    string output_path = "F/" + base_name + ".csv";
    int last_l = pow(10, 5);
    int thres_rem_to_ignore = 1; //threshold remainder to ignore

    // Adjust data
    // RWの開始を原点スタート(0スタート)にする、平行移動。
    double start_point = RW_list.front();
    transform(RW_list.begin(), RW_list.end(), RW_list.begin(), [start_point](double y) { return y - start_point; });

    
    int N = RW_list.size();
    vector<pair<int, double>> records_l_F;
    int l;
    for (l = first_l ; l < last_l ; ++l) {
        if (N % l >= thres_rem_to_ignore) continue;

        int num_segments = N / l;
        int N_used = num_segments * l; // Number of data to use
        vector<vector<double>> segments = generate_segments(RW_list, l);
        ///////////////////////////////////////////////assert statement
        // Calculate the total number of elements in segments
        int total_elements = 0;
        for (const vector<double>& segment : segments) {
            total_elements += segment.size();
        }
        // Check if the number of elements in segments matches N_used
        if (total_elements != N_used) {
            cerr << "Error: The number of elements in segments does not match N_used. Number of elements in segments: " << total_elements << ", N_used: " << N_used << endl;
            return 1; // Error exit
        }
        /////////////////////////////////////////////////////

        double sum_sum_variance = 0.0;
        for (vector<double>& segment : segments) {
            tuple<double, double, double> result = find_best_fit(segment);
            double slope = get<0>(result);
            double intercept = get<1>(result);
            double sum_variance = get<2>(result);
            sum_sum_variance += sum_variance;
        }
        // Record
        double F = pow(sum_sum_variance / N_used, 0.5);
        records_l_F.push_back(make_pair(l, F));
    }
    
    // Display recors_l_F with cout
    //for (const pair<int, double>& record : records_l_F) {
        //cout << "l: " << record.first << ", F: " << record.second << endl;
    //}
    //対数にする前に保存。
    ofstream output(output_path);
    for (size_t i = 0; i < records_l_F.size(); ++i) {
        output << records_l_F[i].first << "," << records_l_F[i].second << endl;
    }
    output.close();


    // records_l_Fからlog(l)とlog(F)の対数を作成
    vector<double> log_l(records_l_F.size());
    vector<double> log_F(records_l_F.size());
    for (size_t i = 0; i < records_l_F.size(); ++i) {
        log_l[i] = log(records_l_F[i].first);
        log_F[i] = log(records_l_F[i].second);
    }
    // log(F)とlog(l)に対して線形フィッティングを行い、傾きを取得
    tuple<double, double, double> result = find_best_fit(log_F, log_l);
    double slope = get<0>(result);
    return slope;

}
