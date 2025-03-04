#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <limits>
#include "../utils/power_law_util.hpp"
#include "theoretical_values.hpp"

using namespace std;

// 二重和を計算する関数（long double を使用）
long double calculate_double_sum(const vector<unsigned int>& y) {
    int n = y.size();
    double sum = 0.0;
    double prefix_sum = 0.0;       // y[0] + y[1] + ... + y[i-1] の累積和
    double prefix_sum_squared = 0.0; // y[0]^2 + y[1]^2 + ... + y[i-1]^2 の累積和

    for (int i = 0; i < n; ++i) {
        // sum += y[i] * (prefix_sum) - prefix_sum_squared
        sum += y[i] * prefix_sum - prefix_sum_squared;

        // 累積和を更新
        prefix_sum += y[i];
        prefix_sum_squared += y[i] * y[i];
    }

    return 2.0*sum;
}

#include <cmath> // pow, tgamma

double calculate_theory_simple(
    double alpha,     // パラメータα
    
    const int n  // データy（サイズn）
) {
    
    double mu = alpha / (alpha - 1.0);
    double c = abs(tgamma(1 - alpha)) * pow(1.0, alpha);
    double theory = 0.0;

    
    for (int i = 1; i < n+1; ++i) {
        // 進捗率を0.1%ごとに表示
        if (i % (n/1000) == 0) {
            cout << "\rProgress: " << fixed << setprecision(1) << (i * 100.0 / n) << "%" << flush;
        }

        for (int j = 1; j < i; ++j) {  // j < i の範囲のみ計算
            const double delta = i - j;  // Δ = i-j

            // 各項の計算
            const double term1 = j*delta /(mu*mu);
            

            theory += term1;
        }
    }

    cout << "\rProgress: 100.0%" << endl;
    return 2*theory;
}



double calculate_theory_recent(
    double alpha,     // パラメータα
    
    const int n  // データy（サイズn）
) {
    
    double mu = alpha / (alpha - 1.0);
    double c = abs(tgamma(1 - alpha)) * pow(1.0, alpha);
    double theory = 0.0;

    // 事前計算: Γ関数を含む係数
    const double gamma_2ma = tgamma(2 - alpha);
    const double gamma_4ma = tgamma(4 - alpha);
    const double gamma_3ma = tgamma(3 - alpha);

    for (int i = 1; i < n+1; ++i) {
        // 進捗率を0.1%ごとに表示
        if (i % (n/1000) == 0) {
            cout << "\rProgress: " << fixed << setprecision(1) << (i * 100.0 / n) << "%" << flush;
        }

        for (int j = 1; j < i; ++j) {  // j < i の範囲のみ計算
            const double delta = i - j;  // Δ = i-j

            // 各項の計算
            const double term1 = j*delta /(mu*mu);
            const double term2 = (c / (2 * pow(mu, 3))) * (pow(j, 1 - alpha) * delta * delta) / gamma_2ma;
            const double term3 = -(c * pow(delta, 3 - alpha)) / (pow(mu, 3) * gamma_4ma);
            const double term4 = (c / pow(mu, 3)) * (pow(j, 2 - alpha) * delta) / gamma_3ma;


            theory += term1 + term2 + term3 + term4;
        }
    }

    cout << "\rProgress: 100.0%" << endl;
    return 2*theory;
}


double calculate_theory_2_case(
    double alpha,     // パラメータα
    
    const int n  // データy（サイズn）
) {
    double mu = alpha / (alpha - 1.0);
    double c = abs(tgamma(1 - alpha)) * pow(1.0, alpha);
    double theory = 0.0;

    // 事前計算: Γ関数を含む係数
    const double gamma_2ma = tgamma(2 - alpha);
    const double gamma_4ma = tgamma(4 - alpha);
    const double gamma_3ma = tgamma(3 - alpha);

    for (int i = 1; i < n+1; ++i) {
        // 進捗率を0.1%ごとに表示
        if (i % (n/1000) == 0) {
            cout << "\rProgress: " << fixed << setprecision(1) << (i * 100.0 / n) << "%" << flush;
        }

        for (int j = 1; j < i; ++j) {  // j < i の範囲のみ計算
            const double delta = i - j;  // Δ = i-j

            // 各項の計算
            const double term1 = j*delta /(mu*mu);
            double term2 = 0.0;
            double term3 = 0.0;
            double term4 = 0.0;
            if (j > delta) {
                term2 = (c / (2 * pow(mu, 3))) * (pow(j, 1 - alpha) * delta * delta) / gamma_2ma;
                term3 = -(c * pow(delta, 3 - alpha)) / (pow(mu, 3) * gamma_4ma);
                term4 = (c / pow(mu, 3)) * (pow(j, 2 - alpha) * delta) / gamma_3ma;
            } else {
                term2 = (c / (2 * pow(mu, 3))) * (pow(delta, 1 - alpha) * j * j) / gamma_2ma;
                term3 = -(c * pow(j, 3 - alpha)) / (pow(mu, 3) * gamma_4ma);
                term4 = (c / pow(mu, 3)) * (pow(delta, 2 - alpha) * j) / gamma_3ma;
            }


            theory += term1 + term2 + term3 + term4;
        }
    }

    cout << "\rProgress: 100.0%" << endl;
    return 2*theory;
}


double calculate_theory_3_case(
    double alpha,     // パラメータα
    
    const int n  // データy（サイズn）
) {
    double mu = alpha / (alpha - 1.0);
    double c = abs(tgamma(1 - alpha)) * pow(1.0, alpha);
    double theory = 0.0;

    // 事前計算: Γ関数を含む係数
    const double gamma_2ma = tgamma(2 - alpha);
    const double gamma_4ma = tgamma(4 - alpha);
    const double gamma_3ma = tgamma(3 - alpha);

    for (int i = 1; i < n+1; ++i) {
        // 進捗率を0.1%ごとに表示
        if (i % (n/1000) == 0) {
            cout << "\rProgress: " << fixed << setprecision(1) << (i * 100.0 / n) << "%" << flush;
        }

        for (int j = 1; j < i; ++j) {  // j < i の範囲のみ計算
            const double delta = i - j;  // Δ = i-j

            // 各項の計算
            const double term1 = j*delta /(mu*mu);
            double term2 = 0.0;
            double term3 = 0.0;
            double term4 = 0.0;
            if (j > 1.5*delta) {
                term2 = (c / (2 * pow(mu, 3))) * (pow(j, 1 - alpha) * delta * delta) / gamma_2ma;
                term3 = -(c * pow(delta, 3 - alpha)) / (pow(mu, 3) * gamma_4ma);
                term4 = (c / pow(mu, 3)) * (pow(j, 2 - alpha) * delta) / gamma_3ma;
            } else if (delta > 1.5*j) {
                term2 = (c / (2 * pow(mu, 3))) * (pow(delta, 1 - alpha) * j * j) / gamma_2ma;
                term3 = -(c * pow(j, 3 - alpha)) / (pow(mu, 3) * gamma_4ma);
                term4 = (c / pow(mu, 3)) * (pow(delta, 2 - alpha) * j) / gamma_3ma;
            } else {
                term2 = -(c / pow(mu, 3)) * (pow(j, 3 - alpha) * pow(delta, 2 - alpha)  +  (pow(delta, 3 - alpha) * pow(j, 2 - alpha))) / gamma_3ma;
                term3 = (c / pow(mu, 3)) * (pow(delta, 2 - alpha) * j) / gamma_3ma;
                term4 = (c / pow(mu, 3)) * (pow(j, 2 - alpha) * delta) / gamma_3ma;
            }


            theory += term1 + term2 + term3 + term4;
        }
    }

    cout << "\rProgress: 100.0%" << endl;
    return 2*theory;
}

double calculate_theory_perfect_deepseek(
    double alpha,     // パラメータα
    
    const int n  // データy（サイズn）
) {
    double mu = alpha / (alpha - 1.0);
    double c = abs(tgamma(1 - alpha)) * pow(1.0, alpha);
    double theory = 0.0;

    // 事前計算: Γ関数を含む係数
    const double gamma_2ma = tgamma(2 - alpha);
    const double gamma_4ma = tgamma(4 - alpha);
    const double gamma_3ma = tgamma(3 - alpha);

    for (int i = 1; i < n+1; ++i) {
        // 進捗率を0.1%ごとに表示
        if (i % (n/1000) == 0) {
            cout << "\rProgress: " << fixed << setprecision(1) << (i * 100.0 / n) << "%" << flush;
        }

        for (int j = 1; j < i; ++j) {  // j < i の範囲のみ計算
            const double delta = i - j;  // Δ = i-j

            // 各項の計算
            const double term1 = j*delta /(mu*mu);
            double term2_1 = 0.0;
            double term2_2 = 0.0;
            double term2 = 0.0;
            double term3 = 0.0;
            double term4 = 0.0;
            
                        
            if (j > 10.0*delta) {
                term2 = (c / (2 * pow(mu, 3))) * (pow(j, 1 - alpha) * delta * delta) / gamma_2ma;
                term3 = -(c * pow(delta, 3 - alpha)) / (pow(mu, 3) * gamma_4ma);
                term4 = (c / pow(mu, 3)) * (pow(j, 2 - alpha) * delta) / gamma_3ma;
            } else {

            term2_1 = (c / (pow(mu, 3))) * (pow(j, 1 - alpha) * delta * delta)*(exp(j/delta) - 1) / gamma_3ma;
            term2_2 = (c / (pow(mu, 3))) * pow(j, 3 - alpha) / (gamma_3ma*gamma_4ma);
            term2 = - term2_1 - term2_2;
            term3 = (c / pow(mu, 3)) * (pow(delta, 2 - alpha) * j) / gamma_3ma;
            term4 = (c / pow(mu, 3)) * (pow(j, 2 - alpha) * delta) / gamma_3ma;
            }
        


            theory += term1 + term2 + term3 + term4;
        }
    }

    cout << "\rProgress: 100.0%" << endl;
    return 2*theory;
}


double calculate_theory_3_case_confident(
    double alpha,     // パラメータα
    
    const int n  // データy（サイズn）
) {
    double mu = alpha / (alpha - 1.0);
    double c = abs(tgamma(1 - alpha)) * pow(1.0, alpha);
    double theory = 0.0;

    // 事前計算: Γ関数を含む係数
    const double gamma_2ma = tgamma(2 - alpha);
    const double gamma_4ma = tgamma(4 - alpha);
    const double gamma_3ma = tgamma(3 - alpha);

    for (int i = 1; i < n+1; ++i) {
        // 進捗率を0.1%ごとに表示
        if (i % (n/1000) == 0) {
            cout << "\rProgress: " << fixed << setprecision(1) << (i * 100.0 / n) << "%" << flush;
        }

        for (int j = 1; j < i; ++j) {  // j < i の範囲のみ計算
            const double delta = i - j;  // Δ = i-j

            // 各項の計算
            const double term1 = j*delta /(mu*mu);
            double term2 = 0.0;
            double term3 = 0.0;
            double term4 = 0.0;
            if (j > 2.0*delta) {
                term2 = (c / (2 * pow(mu, 3))) * (pow(j, 1 - alpha) * delta * delta) / gamma_2ma;
                term3 = -(c * pow(delta, 3 - alpha)) / (pow(mu, 3) * gamma_4ma);
                term4 = (c / pow(mu, 3)) * (pow(j, 2 - alpha) * delta) / gamma_3ma;
            } else if (delta > 2.0*j) {
                term2 = (c / (2 * pow(mu, 3))) * (pow(delta, 1 - alpha) * j * j) / gamma_2ma;
                term3 = -(c * pow(j, 3 - alpha)) / (pow(mu, 3) * gamma_4ma);
                term4 = (c / pow(mu, 3)) * (pow(delta, 2 - alpha) * j) / gamma_3ma;
            } else {
                term2 = 0.0;
                term3 = (c / pow(mu, 3)) * (pow(delta, 2 - alpha) * j) / gamma_3ma;
                term4 = (c / pow(mu, 3)) * (pow(j, 2 - alpha) * delta) / gamma_3ma;
            }


            theory += term1 + term2 + term3 + term4;
        }
    }

    cout << "\rProgress: 100.0%" << endl;
    return 2*theory;
}


double calculate_theory_3_case_confident_15(
    double alpha,     // パラメータα
    
    const int n  // データy（サイズn）
) {
    double mu = alpha / (alpha - 1.0);
    double c = abs(tgamma(1 - alpha)) * pow(1.0, alpha);
    double theory = 0.0;

    // 事前計算: Γ関数を含む係数
    const double gamma_2ma = tgamma(2 - alpha);
    const double gamma_4ma = tgamma(4 - alpha);
    const double gamma_3ma = tgamma(3 - alpha);

    for (int i = 1; i < n+1; ++i) {
        // 進捗率を0.1%ごとに表示
        if (i % (n/1000) == 0) {
            cout << "\rProgress: " << fixed << setprecision(1) << (i * 100.0 / n) << "%" << flush;
        }

        for (int j = 1; j < i; ++j) {  // j < i の範囲のみ計算
            const double delta = i - j;  // Δ = i-j

            // 各項の計算
            const double term1 = j*delta /(mu*mu);
            double term2 = 0.0;
            double term3 = 0.0;
            double term4 = 0.0;
            if (j > 15.0*delta) {
                term2 = (c / (2 * pow(mu, 3))) * (pow(j, 1 - alpha) * delta * delta) / gamma_2ma;
                term3 = -(c * pow(delta, 3 - alpha)) / (pow(mu, 3) * gamma_4ma);
                term4 = (c / pow(mu, 3)) * (pow(j, 2 - alpha) * delta) / gamma_3ma;
            } else if (delta > 15.0*j) {
                term2 = (c / (2 * pow(mu, 3))) * (pow(delta, 1 - alpha) * j * j) / gamma_2ma;
                term3 = -(c * pow(j, 3 - alpha)) / (pow(mu, 3) * gamma_4ma);
                term4 = (c / pow(mu, 3)) * (pow(delta, 2 - alpha) * j) / gamma_3ma;
            } else {
                term2 = 0.0;
                term3 = (c / pow(mu, 3)) * (pow(delta, 2 - alpha) * j) / gamma_3ma;
                term4 = (c / pow(mu, 3)) * (pow(j, 2 - alpha) * delta) / gamma_3ma;
            }


            theory += term1 + term2 + term3 + term4;
        }
    }

    cout << "\rProgress: 100.0%" << endl;
    return 2*theory;
}


double calculate_theory_1_case(
    double alpha,     // パラメータα
    
    const int n  // データy（サイズn）
) {
    double mu = alpha / (alpha - 1.0);
    double c = abs(tgamma(1 - alpha)) * pow(1.0, alpha);
    double theory = 0.0;

    // 事前計算: Γ関数を含む係数

    for (int i = 1; i < n+1; ++i) {
        // 進捗率を0.1%ごとに表示
        if (i % (n/1000) == 0) {
            cout << "\rProgress: " << fixed << setprecision(1) << (i * 100.0 / n) << "%" << flush;
        }

        for (int j = 1; j < i; ++j) {  // j < i の範囲のみ計算
            const double delta = i - j;  // Δ = i-j

            // 各項の計算
            const double term1 = j*delta /(mu*mu);
            double term2 = 0.0;
            double term3 = 0.0;
            double term4 = 0.0;
            
            term2 = ((j*pow(j+delta, 2-alpha)) - pow(j, 3-alpha))/(pow(mu, 3)*(alpha-1)*(2-alpha));
            term3 = 0.0;
            term4 = 0.0;
        

            theory += term1 + term2 + term3 + term4;
        }
    }

    cout << "\rProgress: 100.0%" << endl;
    return 2*theory;
}


double calculate_theory_from_origin(
    double alpha,     // パラメータα
    
    const int n  // データy（サイズn）
) {
    double mu = alpha / (alpha - 1.0);
    double c = abs(tgamma(1 - alpha)) * pow(1.0, alpha);
    double gamma_4ma = tgamma(4 - alpha);
    double theory = 0.0;

    // 事前計算: Γ関数を含む係数

    for (int i = 1; i < n+1; ++i) {
        // 進捗率を0.1%ごとに表示
        if (i % (n/1000) == 0) {
            cout << "\rProgress: " << fixed << setprecision(1) << (i * 100.0 / n) << "%" << flush;
        }

        for (int j = 1; j < i; ++j) {  // j < i の範囲のみ計算
            const double delta = i - j;  // Δ = i-j

            // 各項の計算
            const double term1 = j*delta /(mu*mu);
            double term2 = 0.0;
            double term3 = 0.0;
            double term4 = 0.0;
            
            term2 = 0.0;
            term3 = -(c * ((pow(delta, 3.0 - alpha)) + pow(j, 3.0 - alpha) - (pow(delta + j, 3.0 - alpha)))) / (pow(mu, 3) * gamma_4ma); 
            term4 = 0.0;
        

            theory += term1 + term2 + term3 + term4;
        }
    }

    cout << "\rProgress: 100.0%" << endl;
    return 2*theory;
}


int main() {
    // 出力の精度を最大に設定
    cout << setprecision(numeric_limits<double>::max_digits10) << scientific;

    double tau_0 = 1.0;
    int sample_amount = pow(10, 5);  // サンプル数を10^7に設定
    double T = sample_amount * tau_0;
    double alpha = 1.5;  // 固定のalpha値

    TheoreticalValueManager theoretical_manager;

    double total_sum = 0.0;
    int repeat_amount = 10000;
    

    // 理論値の計算と保存/再利用
    auto calculate_or_get_theoretical = [&](const string& method_name, auto calculation_func) {
        if (auto cached_value = theoretical_manager.getValue(alpha, sample_amount, method_name)) {
            cout << "Using cached value for " << method_name << endl;
            return *cached_value;
        }
        double value = calculation_func();
        theoretical_manager.setValue(alpha, sample_amount, method_name, value);
        return value;
    };

    double average = pow(10, 9);

    // 各理論値の計算または取得
    double theoretical_simple = calculate_or_get_theoretical("simple", 
        [&]() { return calculate_theory_simple(alpha, sample_amount); });
    cout << "Theoretical value simple: " << scientific << theoretical_simple << endl;
    cout << "Relative error simple: " << (theoretical_simple - average)/average << endl;

    double theoretical = calculate_or_get_theoretical("recent", 
        [&]() { return calculate_theory_recent(alpha, sample_amount); });
    cout << "Theoretical value recent: " << scientific << theoretical << endl;
    cout << "Relative error recent: " << (theoretical - average)/average << endl;
    
    double theoretical_2_case = calculate_or_get_theoretical("2_case", 
        [&]() { return calculate_theory_2_case(alpha, sample_amount); });
    cout << "Theoretical value 2 case: " << scientific << theoretical_2_case << endl;
    cout << "Relative error 2 case: " << (theoretical_2_case - average)/average << endl;

    double theoretical_3_case = calculate_or_get_theoretical("3_case", 
        [&]() { return calculate_theory_3_case(alpha, sample_amount); });
    cout << "Theoretical value 3 case: " << scientific << theoretical_3_case << endl;
    cout << "Relative error 3 case: " << (theoretical_3_case - average)/average << endl;

    double theoretical_perfect_deepseek = calculate_or_get_theoretical("perfect_deepseek", 
        [&]() { return calculate_theory_perfect_deepseek(alpha, sample_amount); });
    cout << "Theoretical value perfect deepseek: " << scientific << theoretical_perfect_deepseek << endl;
    cout << "Relative error perfect deepseek: " << (theoretical_perfect_deepseek - average)/average << endl;

    double theoretical_3_case_confident = calculate_or_get_theoretical("3_case_confident", 
        [&]() { return calculate_theory_3_case_confident(alpha, sample_amount); });
    cout << "Theoretical value 3 case confident: " << scientific << theoretical_3_case_confident << endl;
    cout << "Relative error 3 case confident: " << (theoretical_3_case_confident - average)/average << endl;

    double theoretical_3_case_confident_15 = calculate_or_get_theoretical("3_case_confident_15", 
        [&]() { return calculate_theory_3_case_confident_15(alpha, sample_amount); });
    cout << "Theoretical value 3 case confident 15: " << scientific << theoretical_3_case_confident_15 << endl;
    cout << "Relative error 3 case confident 15: " << (theoretical_3_case_confident_15 - average)/average << endl;

    double theoretical_1_case = calculate_or_get_theoretical("1_case", 
        [&]() { return calculate_theory_1_case(alpha, sample_amount); });
    cout << "Theoretical value 1 case: " << scientific << theoretical_1_case << endl;
    cout << "Relative error 1 case: " << (theoretical_1_case - average)/average << endl;

    double theoretical_from_origin = calculate_or_get_theoretical("from_origin", 
        [&]() { return calculate_theory_from_origin(alpha, sample_amount); });
    cout << "Theoretical value from origin: " << scientific << theoretical_from_origin << endl;
    cout << "Relative error from origin: " << (theoretical_from_origin - average)/average << endl;

    // 結果表示時は最大精度で
    cout << scientific << setprecision(numeric_limits<double>::max_digits10);

    return 0;
}
