#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <fstream>
#include <random>
#include <ctime>

using namespace std;
using namespace std::chrono;

// 1つ目の実装: randを使用
namespace impl1 {
    double waiting_time_power_law(double tau_0, double alpha) {
        double u = ((double) rand())/((double) RAND_MAX);
        if (u < 1e-300) {
            u = ((double) rand())/(((double) RAND_MAX)*((double) RAND_MAX));
            if (u < 1e-300) {
                u = ((double) rand())/(((double) RAND_MAX)*((double) RAND_MAX)*((double) RAND_MAX));
                if (u < 1e-300) {
                    u = ((double) rand())/(((double) RAND_MAX)*((double) RAND_MAX)*((double) RAND_MAX)*((double) RAND_MAX));
                }
            }
        }
        return tau_0 * std::pow(u, -1.0 / alpha);
    }

    vector<unsigned int> generate_power_law_point_process(double alpha, double tau_0, int sample_amount) {
        vector<unsigned int> event_counts(sample_amount, 0);
        double t = 0.0;
        unsigned int count = 0;

        while (t < sample_amount) {
            double waiting_time = waiting_time_power_law(tau_0, alpha);
            t += waiting_time;
            if (t >= sample_amount) break;
            
            int time_index = static_cast<int>(t);
            if (time_index < sample_amount) {
                count++;
                event_counts[time_index] = count;
            }
        }

        // 累積カウントを埋める
        for (int i = 1; i < sample_amount; ++i) {
            if (event_counts[i] == 0) {
                event_counts[i] = event_counts[i-1];
            }
        }

        return event_counts;
    }
}

// 2つ目の実装: Xorshift128を使用
namespace impl2 {
    class Xorshift128 {
    public:
        Xorshift128(uint64_t seed) {
            seed_ = seed;
            if (seed_ == 0) seed_ = 123456789;
            x_ = seed_;
            y_ = 362436069;
            z_ = 521288629;
            w_ = 88675123;
        }

        double nextDouble() {
            uint64_t t = x_ ^ (x_ << 11);
            x_ = y_; y_ = z_; z_ = w_;
            w_ = (w_ ^ (w_ >> 19)) ^ (t ^ (t >> 8));
            return static_cast<double>(w_ & 0xFFFFFFFFFFFFF) / 0x10000000000000;
        }

    private:
        uint64_t seed_, x_, y_, z_, w_;
    };

    double waiting_time_power_law(Xorshift128& rng, double tau_0, double alpha) {
        double u = rng.nextDouble();
        return tau_0 * std::pow(u, -1.0 / alpha);
    }

    vector<unsigned int> generate_power_law_point_process(double alpha, double tau_0, int sample_amount) {
        Xorshift128 rng(static_cast<uint64_t>(time(NULL)));
        vector<unsigned int> event_counts(sample_amount, 0);
        double t = 0.0;
        unsigned int count = 0;

        while (t < sample_amount) {
            double waiting_time = waiting_time_power_law(rng, tau_0, alpha);
            t += waiting_time;
            if (t >= sample_amount) break;
            
            int time_index = static_cast<int>(t);
            if (time_index < sample_amount) {
                count++;
                event_counts[time_index] = count;
            }
        }

        // 累積カウントを埋める
        for (int i = 1; i < sample_amount; ++i) {
            if (event_counts[i] == 0) {
                event_counts[i] = event_counts[i-1];
            }
        }

        return event_counts;
    }
}

void measure_speed(double alpha, double tau_0, int sample_amount, int num_trials) {
    cout << "===== 速度測定開始 =====" << endl;
    cout << "パラメータ設定:" << endl;
    cout << "alpha: " << alpha << endl;
    cout << "tau_0: " << tau_0 << endl;
    cout << "sample_amount: " << sample_amount << endl;
    cout << "試行回数: " << num_trials << endl;
    cout << endl;

    // 実装1の測定
    vector<double> impl1_times;
    for (int i = 0; i < num_trials; ++i) {
        auto start = high_resolution_clock::now();
        auto result = impl1::generate_power_law_point_process(alpha, tau_0, sample_amount);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        impl1_times.push_back(duration.count());
        
        cout << "実装1 (rand使用) 試行 " << i + 1 << ": " << duration.count() << " ms" << endl;
    }

    // 実装2の測定
    vector<double> impl2_times;
    for (int i = 0; i < num_trials; ++i) {
        auto start = high_resolution_clock::now();
        auto result = impl2::generate_power_law_point_process(alpha, tau_0, sample_amount);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        impl2_times.push_back(duration.count());
        
        cout << "実装2 (Xorshift128使用) 試行 " << i + 1 << ": " << duration.count() << " ms" << endl;
    }

    // 平均時間の計算
    double impl1_avg = 0.0, impl2_avg = 0.0;
    for (int i = 0; i < num_trials; ++i) {
        impl1_avg += impl1_times[i];
        impl2_avg += impl2_times[i];
    }
    impl1_avg /= num_trials;
    impl2_avg /= num_trials;

    // 結果の出力
    cout << "\n===== 測定結果 =====" << endl;
    cout << "実装1 (rand使用) 平均実行時間: " << fixed << setprecision(2) << impl1_avg << " ms" << endl;
    cout << "実装2 (Xorshift128使用) 平均実行時間: " << impl2_avg << " ms" << endl;
    cout << "速度比 (impl2/impl1): " << impl2_avg/impl1_avg << endl;

    // 結果をファイルに保存
    ofstream outfile("speed_measurement_results.txt", ios::app);
    outfile << "===== 測定結果 =====" << endl;
    outfile << "パラメータ:" << endl;
    outfile << "alpha: " << alpha << endl;
    outfile << "tau_0: " << tau_0 << endl;
    outfile << "sample_amount: " << sample_amount << endl;
    outfile << "試行回数: " << num_trials << endl;
    outfile << "実装1 (rand使用) 平均実行時間: " << impl1_avg << " ms" << endl;
    outfile << "実装2 (Xorshift128使用) 平均実行時間: " << impl2_avg << " ms" << endl;
    outfile << "速度比 (impl2/impl1): " << impl2_avg/impl1_avg << endl;
    outfile << "\n" << endl;
    outfile.close();
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));  // 実装1用の乱数初期化
    
    // パラメータ設定
    double alpha = 1.5;  // べき指数
    double tau_0 = 1.0;  // 最小待ち時間
    int sample_amount = 10000000;  // サンプル数
    int num_trials = 5;  // 試行回数

    measure_speed(alpha, tau_0, sample_amount, num_trials);

    return 0;
} 