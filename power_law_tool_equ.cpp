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
#include <tuple> 

#include <numeric>
#include <vector>
#include <ctime>
#include <iomanip>

#include "power_law_tool_equ.hpp"

using namespace std;

double waiting_time_power_law(double tau_0, double alpha) {
    double u = ((double) rand())/((double) RAND_MAX);
    return tau_0 * std::pow(u, -1.0 / alpha);
}

double get_exceeded_waiting_time(double alpha, double tau_0, double T) {
// simulate_event_times関数をほぼ真似た形。だが、連続関数を想定した、event_timesというのは必要ないので削除
    long double t = 0.0;   //前回のexceeded_wating_timeを使う。最初は0のはず
    double exceeded_waiting_time = 0.0; // 初期化
    double T100 = T*100;
    
    while (t < T100) {
        double waiting_time = waiting_time_power_law(tau_0, alpha);
        t += waiting_time;
        if (t >= T100) {
            exceeded_waiting_time = t - T100;
            break;// シミュレーション時間を超えたら終了
        }
    }
    time_t now = time(0);
    tm* localtm = localtime(&now);
    cout << put_time(localtm, "%Y-%m-%d %H:%M:%S: ") << T100<< "回 系をEvolve the system over time。空回し (alpha=" << alpha << ")" << endl;
        
    return exceeded_waiting_time;
}




// イベント時刻をシミュレーションする関数
std::vector<double> simulate_event_times(double tau_0, double alpha, double T) {

    //まず系を空で回す
    get_exceeded_waiting_time(alpha, tau_0, T);
    std::vector<double> event_times;
    double t = 0.0;

    while (t < T) {
        double waiting_time = waiting_time_power_law(tau_0, alpha);
        t += waiting_time;
        if (t >= T) break; // シミュレーション時間を超えたら終了
        event_times.push_back(t);
    }

    return event_times;
}

// 各整数時刻での累積イベント数を計算する関数（2つのforループを使用）
std::vector<unsigned long long> count_events_per_unit_time(const std::vector<double>& event_times, int T) {
    std::vector<unsigned long long> event_counts(T, 0); // サイズをTに変更
    std::vector<int> events_per_time(T, 0);   // その時刻()において一秒間の間にイベントが起きた回数

    // 1つ目のforループ：各時刻でのイベント数をカウント
    for (double t : event_times) {
        int time_index = static_cast<int>(t); // floor(t) から変更
        if (time_index < T) {
            events_per_time[time_index]++;
        }
    }

    // 2つ目のforループ：累積イベント数を計算
    event_counts[0] = events_per_time[0];
    for (int i = 1; i < T; ++i) {
        event_counts[i] = event_counts[i - 1] + events_per_time[i];
    }

    return event_counts;
}




// 更新過程をシミュレーションし、各時刻でのイベント数を返す関数
std::vector<unsigned long long> generate_power_law_point_process(double alpha, double tau_0, int sample_amount) {
    // 乱数シードの初期化（必要に応じてシード値を固定してください）
    srand(static_cast<unsigned int>(time(NULL)));

    // イベント時刻のシミュレーション
    std::vector<double> event_times = simulate_event_times(tau_0, alpha, sample_amount);

    // 各時刻での累積イベント数を計算
    std::vector<unsigned long long> event_counts = count_events_per_unit_time(event_times, sample_amount);

    return event_counts;
}





double get_directly_residuls2(const std::vector<unsigned long long>& y) {
    int n = y.size();
    if (n == 0) {
        return 0.0; // または適切なエラー処理
    }

    //double mean = std::reduce(std::execution::par, y.begin(), y.end(), 0.0) / y.size();
    double sum = 0.0;
    long double sq_sum = 0.0L;
    long double weighted_sum = 0.0L;
    
    for (double value : y) {
        sum += value;
    }
    double mean = sum / n;


    for (size_t i = 0; i < y.size(); ++i) {
        double value = y[i];
        sq_sum += value * value;
        weighted_sum += (i+1) * value;
    }

    long double v2_y = (sq_sum / n) - mean*mean;
    long double second_term = (12.0*(n+1)/(n-1))*pow((weighted_sum/n/(n+1) - mean/2),2);

    return v2_y - second_term;
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
        //cout << "コピーしました" << endl;
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

// Function to divide data
vector<vector<unsigned long long>> generate_segments(const vector<unsigned long long>& data, int scale) {
    vector<vector<unsigned long long>> segments;
    int num_segments = data.size() / scale;
    for (int i = 0; i < num_segments; ++i) {
        vector<unsigned long long> segment(data.begin() + i * scale, data.begin() + ((i + 1) * scale));
        segments.push_back(segment);
    }
    return segments;
}



std::tuple<double, double, std::vector<int>, std::vector<double>> dfa(vector<unsigned long long> RW_list, double alpha, int t_first_l, int t_last_l) {
    cout << "dfa関数開始" << endl;
    // alphaを少数第1位で表示するために、snprintfを使用します。
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%.1f", alpha);
    string base_name = buffer;
    
    //string input_path = base_name + ".csv";
    string output_path = "F/" + base_name + ".csv";
    int first_l = t_first_l;
    int last_l = t_last_l;
    int thres_rem_to_ignore = 1; //threshold remainder to ignore

    // Adjust data
    // RWの開始を原点スタート(0スタート)にする、平行移動。
    unsigned long long start_point = RW_list.front();
    transform(RW_list.begin(), RW_list.end(), RW_list.begin(), [start_point](unsigned long long y) { return y - start_point; });

    
    int N = RW_list.size();
    vector<pair<int, double>> records_l_F;
    int l;
    
    for (l = first_l ; l < last_l ; ++l) {
        if (N % l >= thres_rem_to_ignore) continue;

        int num_segments = N / l;
        int N_used = num_segments * l; // Number of data to use
        vector<vector<unsigned long long>> segments = generate_segments(RW_list, l);
        ///////////////////////////////////////////////assert statement
        // Calculate the total number of elements in segments
        int total_elements = 0;
        for (const vector<unsigned long long>& segment : segments) {
            total_elements += segment.size();
        }
        // Check if the number of elements in segments matches N_used
        if (total_elements != N_used) {
            cerr << "Error: The number of elements in segments does not match N_used. Number of elements in segments: " << total_elements << ", N_used: " << N_used << endl;
            // エラー時は意味のない値を返す
            return std::make_tuple(
                std::numeric_limits<double>::quiet_NaN(),  // slope
                std::numeric_limits<double>::quiet_NaN(),  // intercept
                vector<int>(),                             // empty l_values
                vector<double>()                           // empty F_values
            );
        }
        /////////////////////////////////////////////////////

        double sum_variance = 0.0;
        for (vector<unsigned long long>& segment : segments) {
            double avg_variance = get_directly_residuls2(segment);
            sum_variance += avg_variance;
        }
        // Record
        double F = pow(sum_variance / num_segments, 0.5);
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

    for (const auto& record : records_l_F) {
        std::cout << "l: " << record.first << ", F: " << record.second << std::endl;
    }

    vector<int> l_values_all;
    vector<double> F_values_all;
    for (const auto& record : records_l_F) {
        l_values_all.push_back(record.first);
        F_values_all.push_back(record.second);
    }
    
    //pairのベクトルにしてしまったので、それぞれを分ける。
    vector<int> l_values;
    vector<double> F_values;
    for (const auto& record : records_l_F) {
        if (record.first >= pow(10,5)){
            l_values.push_back(record.first);
            F_values.push_back(record.second);
        }
    }

    // records_l_Fからlog(l)とlog(F)の対数を作成
    vector<double> log_l(l_values.size());
    vector<double> log_F(l_values.size());    
    for (size_t i = 0; i < l_values.size(); ++i) {
        log_l[i] = log(l_values[i]);
        log_F[i] = log(F_values[i]);
    }

    // log(F)とlog(l)に対して線形フィッティングを行い、傾きを取得
    tuple<double, double, double> result = find_best_fit(log_F, log_l);
    double slope = get<0>(result);
    double intercept = get<1>(result);
    std::cout << "========================================" << std::endl;
    std::cout << "Slope: " << slope << std::endl;
    std::cout << "Intercept: " << intercept << std::endl;
    std::cout << "========================================" << std::endl;
    return std::make_tuple(slope, intercept, l_values_all, F_values_all);
}