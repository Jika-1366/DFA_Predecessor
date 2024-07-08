#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <iomanip>

double calculate_percentile(std::vector<double>& data, double percentile) {
    size_t index = static_cast<size_t>(percentile * (data.size() - 1));
    return data[index];
}

int main() {
    const int sample_amount = 1000;  // サンプル数
    const double alpha = 2.0;        // べき指数
    const double tau_0 = 1.0;        // スケールファクター
    const int num_bins = 20;         // ヒストグラムのビン数

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    std::vector<double> samples(sample_amount);
    
    for (int i = 0; i < sample_amount; ++i) {
        double u = dis(gen);
        samples[i] = tau_0 * std::pow(u, -1.0 / alpha);
    }

    // 第三四分位数を計算
    std::sort(samples.begin(), samples.end());
    double q3 = calculate_percentile(samples, 0.75);

    // 第三四分位数以下のデータのみを使用
    std::vector<double> filtered_samples;
    for (double sample : samples) {
        if (sample <= q3) {
            filtered_samples.push_back(sample);
        }
    }

    // ヒストグラムの範囲を決定
    double min_val = *std::min_element(filtered_samples.begin(), filtered_samples.end());
    double max_val = *std::max_element(filtered_samples.begin(), filtered_samples.end());
    double bin_width = (max_val - min_val) / num_bins;

    // ヒストグラムを作成
    std::vector<int> histogram(num_bins, 0);
    for (double sample : filtered_samples) {
        int bin = static_cast<int>((sample - min_val) / bin_width);
        if (bin >= 0 && bin < num_bins) {
            histogram[bin]++;
        }
    }

    // ヒストグラムを表示
    std::cout << "ヒストグラム:\n";
    for (int i = 0; i < num_bins; ++i) {
        double bin_start = min_val + i * bin_width;
        double bin_end = bin_start + bin_width;
        std::cout << std::fixed << std::setprecision(2) 
                  << "[" << bin_start << ", " << bin_end << "): " 
                  << std::string(histogram[i], '*') << "\n";
    }

    return 0;
}