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

#include "power_law_tool_7.hpp"

using namespace std;


double waiting_time_power_law(double tau_0, double alpha) {
    double u = ((double) rand())/((double) RAND_MAX);
    return tau_0 * std::pow(u, -1.0 / alpha);
}


// イベント時刻をシミュレーションする関数
std::pair<std::vector<double>, double> simulate_event_times(double tau_0, double alpha, double T, double exceeded_waiting_time_parameter) {
    std::vector<double> event_times;
    double t = exceeded_waiting_time_parameter;   //前回のexceeded_wating_timeを使う。最初は0のはず
    double exceeded_waiting_time = 0.0; // 初期化

    while (t < T) {
        double waiting_time = waiting_time_power_law(tau_0, alpha);
        t += waiting_time;
        if (t >= T) {
            exceeded_waiting_time = t - T;
            break;// シミュレーション時間を超えたら終了
        }
        event_times.push_back(t);
    }

    return std::make_pair(event_times, exceeded_waiting_time);
}


// 各整数時刻での累積イベント数を計算する関数（2つのforループを使用）
std::vector<double> count_events_per_unit_time(const std::vector<double>& event_times, int T) {
    std::vector<double> event_counts(T+1, 0.0); // サイズをT+1に
    std::vector<int> events_per_time(T, 0);   // サイズをTに
    // 1つ目のforループ：各時刻でのイベント数をカウント
    for (double t : event_times) {
        int time_index = static_cast<int>(t); // floor(t) と同義
        if (time_index < T) {
            events_per_time[time_index]++;   //events_per_time[1]は時刻1~2で起きたイベント。これで合っている。
        }
    }
    // 2つ目のforループ：累積イベント数を計算
    event_counts[0] = events_per_time[0];
    for (int i = 1; i < T+1; ++i) {
        event_counts[i] = event_counts[i - 1] + events_per_time[i]; //分かりにくいですが最終時刻はT-1です。この分かりにくさによりsample_amountは分かりやすい数になります
    }

    return event_counts;
}

// 更新過程をシミュレーションし、各時刻でのイベント数と超過待ち時間を返す関数
std::pair<std::vector<double>, double> generate_power_law_point_process(double alpha, double tau_0, int sample_amount, double exceeded_waiting_time_parameter) {
    // 乱数シードの初期化（必要に応じてシード値を固定してください）
    srand(static_cast<unsigned int>(time(NULL)));

    // イベント時刻のシミュレーション
    std::pair<std::vector<double>, double> result = simulate_event_times(tau_0, alpha, sample_amount, exceeded_waiting_time_parameter);
    std::vector<double> event_times = result.first;
    double exceeded_waiting_time_next = result.second; // パラメータ名と異なる変数名を使用

    // 各時刻での累積イベント数を計算
    std::vector<double> event_counts = count_events_per_unit_time(event_times, sample_amount);
    
    /////////////////////////////////////////////
    //ofstream ofs_event_counts("event_counts.txt");
    //for (int i = 0; i < event_counts.size(); ++i) {
        //ofs_event_counts << event_counts[i] << endl;
    //}
    //ofstream ofs_exceeded_waiting_time_next("exceeded_waiting_time_next.txt");
    //ofs_exceeded_waiting_time_next << exceeded_waiting_time_next << endl;
    ////////////////////////////////////////////////
    return std::make_pair(event_counts, exceeded_waiting_time_next);
}




std::vector<double> incorrect_generate_power_law_point_process(double alpha, double tau_0, int sample_amount) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    srand(time(NULL));
    //double u = dis(gen);
    
    std::vector<double> N(sample_amount);
    N[0] = 0; 
    //waiting_timeの取得
    //double u = dis(gen);
    double u = ((double) rand())/((double) RAND_MAX);
    double waiting_time = tau_0 * std::pow(u, -1.0 / alpha);
    //tとは時刻のこと。時刻の積み重ねがinter_occurence_timeを越えたら、Nを+1して、積み重ねをリセット
    int accumulated_time = 0;
    for (int t = 1; t < sample_amount; ++t) {
        accumulated_time += 1;
        if (accumulated_time >= waiting_time){ // 変更: queueの先頭から値を取得
            accumulated_time = 0;
            N[t]= N[t-1] +1; 
            double u = ((double) rand())/((double) RAND_MAX);
            waiting_time = tau_0 * std::pow(u, -1.0 / alpha);
            }
            
        else{N[t] = N[t-1];
        }

    }
    
    return N;
}    



// 傾き、切片、残差の二乗の合計を返す。yは必須だが、xは渡しても渡さなくともいい。
std::tuple<double, double, double> find_best_fit(const std::vector<double>& y, const std::vector<double>& x) {
    int n = y.size();
    std::vector<double> x_values;

    // xが空の時だけ、適当に作成。
    if (x.empty()) {
        x_values.reserve(n);
        for (int i = 0; i < n; ++i) {
            x_values.push_back(i);  // x = 0,1, 2, ..., n-1(n=T+1なので、x=Tがend.)
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

    if (std::isnan(slope) || std::isnan(intercept)) {
        // 現在時刻を取得
        time_t timer;
        time(&timer);
        struct tm *localTime = localtime(&timer);

        // ファイル名を作成
        char buffer[80];
        strftime(buffer, 80, "error_reports/error_report_%Y%m%d_%H%M%S.txt", localTime);
        string error_report_filename(buffer);

        // エラーレポートをファイルに保存
        ofstream error_report(error_report_filename);
        error_report << "Error: NaN value detected in find_best_fit." << endl;
        error_report << "n: " << n << endl;
        error_report << "x_mean: " << x_mean << endl;
        error_report << "y_mean: " << y_mean << endl;
        error_report << "numerator: " << numerator << endl;
        error_report << "denominator: " << denominator << endl;
        error_report << "x_values: ";
        for (double val : x_values) error_report << val << " ";
        error_report << endl;
        error_report << "y_values: ";
        for (double val : y) error_report << val << " ";
        error_report << endl;
        error_report.close();

       // デバッグ用に標準出力にもエラーメッセージを出力
        cerr << "Error: NaN value detected in find_best_fit. Check error_report.txt for details." << endl;
    }


    // 残差の二乗の合計を計算
    double residual_sum_squares = 0.0;
    for (int i = 0; i < n; ++i) {
        double residual = y[i] - (slope * x_values[i] + intercept);
        residual_sum_squares += residual * residual;
    }

    return std::make_tuple(slope, intercept, residual_sum_squares);
}




double get_sum_squared_residuals(std::vector<double> walk){
    tuple<double, double, double> result = find_best_fit(walk);
    double sum_variance = get<2>(result);
    return sum_variance;
}


std::vector<std::pair<int, double>> calculate_F_values(double alpha, double tau_0, int number_of_segments, int first_i, int last_i, float l_base) {
    std::vector<std::pair<int, double>> l_F_pairs;
    for (int i = first_i; i <= last_i; ++i) {
        int l = static_cast<int>(pow(l_base, i));
        double exceeded_waiting_time = 0.0;
        double sum_squared_residuals = 0.0;
        for (int j = 0; j < number_of_segments; ++j) {
            std::pair<std::vector<double>, double> walk_result = generate_power_law_point_process(alpha, tau_0, l, exceeded_waiting_time);
            std::vector<double> walk = walk_result.first;
            exceeded_waiting_time = walk_result.second;
            sum_squared_residuals += get_sum_squared_residuals(walk);
        }
        
        // F2の計算前にチェック
        if (number_of_segments <= 0 || l <= 0) {
            // 現在時刻を取得
            time_t timer;
            time(&timer);
            struct tm *localTime = localtime(&timer);

            // ファイル名を作成
            char buffer[80];
            strftime(buffer, 80, "error_reports/error_report_%Y%m%d_%H%M%S.txt", localTime);
            string error_report_filename(buffer);

            // エラーレポートをファイルに保存
            ofstream error_report(error_report_filename);
            error_report << "Error: Invalid values for F2 calculation." << endl;
            error_report << "number_of_segments: " << number_of_segments << endl;
            error_report << "l: " << l << endl;
            error_report << "sum_squared_residuals: " << sum_squared_residuals << endl;
            error_report.close();

            cerr << "Error: Invalid values for F2 calculation. Check error_report.txt for details." << endl;
            continue;  // このイテレーションをスキップ
        }

        double F2 = sum_squared_residuals / (number_of_segments*l);
        
        // F2が負またはNaNの場合をチェック
        if (F2 < 0 || std::isnan(F2)) {
            // 現在時刻を取得
            time_t timer;
            time(&timer);
            struct tm *localTime = localtime(&timer);

            // ファイル名を作成
            char buffer[80];
            strftime(buffer, 80, "error_reports/error_report_%Y%m%d_%H%M%S.txt", localTime);
            string error_report_filename(buffer);

            // エラーレポートをファイルに保存
            ofstream error_report(error_report_filename);
            error_report << "Error: Invalid F2 value." << endl;
            error_report << "F2: " << F2 << endl;
            error_report << "sum_squared_residuals: " << sum_squared_residuals << endl;
            error_report << "number_of_segments: " << number_of_segments << endl;
            error_report << "l: " << l << endl;
            error_report.close();

            cerr << "Error: Invalid F2 value. Check error_report.txt for details." << endl;
            continue;  // このイテレーションをスキップ
        }

        double F = pow(F2, 0.5);
        
        // Fの値をチェック
        if (std::isnan(F)) {
            // 現在時刻を取得
            time_t timer;
            time(&timer);
            struct tm *localTime = localtime(&timer);

            // ファイル名を作成
            char buffer[80];
            strftime(buffer, 80, "error_reports/error_report_%Y%m%d_%H%M%S.txt", localTime);
            string error_report_filename(buffer);

            // エラーレポートをファイルに保存
            ofstream error_report(error_report_filename);
            error_report << "Error: NaN value detected in F calculation." << endl;
            error_report << "F2: " << F2 << endl;
            error_report << "F: " << F << endl;
            error_report.close();

            cerr << "Error: NaN value detected in F calculation. Check error_report.txt for details." << endl;
            continue;  // このイテレーションをスキップ
        }

        l_F_pairs.push_back(std::make_pair(l, F));
        cout << "セグメント " <<  number_of_segments << "回 完了 (alpha=" << alpha << ", l=" << l << ")" << endl;
    }

    for (const auto& pair : l_F_pairs) {
        std::cout << "l: " << pair.first << ", F: " << pair.second << std::endl;
    }
    return l_F_pairs;
}
