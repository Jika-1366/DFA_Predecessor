#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include "dfa_methods.hpp"
#include "../power_law_tool_7.hpp"

void print_benchmark_results(const std::string& method_name, double duration_us, double result, double base_result, double base_duration) {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << method_name << ":\n";
    std::cout << "  実行時間: " << duration_us << " マイクロ秒\n";
    std::cout << "  速度比: " << base_duration / duration_us << "倍\n";
    std::cout << "  結果: " << result << "\n";
    std::cout << "  基準との差: " << std::abs(result - base_result) << "\n\n";
}

void run_benchmark(const std::vector<double>& data) {
    std::cout << "データサイズ: " << data.size() << "点\n\n";

    // Legacy手法（基準）
    auto start = std::chrono::high_resolution_clock::now();
    double legacy_result = get_residuals_legacy(data);
    auto end = std::chrono::high_resolution_clock::now();
    auto legacy_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // 直接手法
    start = std::chrono::high_resolution_clock::now();
    double direct_result = get_residuals_direct(data);
    end = std::chrono::high_resolution_clock::now();
    auto direct_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // OpenMP並列処理版
    start = std::chrono::high_resolution_clock::now();
    double openmp_result = get_residuals_openmp(data);
    end = std::chrono::high_resolution_clock::now();
    auto openmp_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // SIMD並列処理版
    start = std::chrono::high_resolution_clock::now();
    double simd_result = get_residuals_simd(data);
    end = std::chrono::high_resolution_clock::now();
    auto simd_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Power Law Tool 7版
    start = std::chrono::high_resolution_clock::now();
    double power_law_7_result = get_residuals_power_law_7(data);
    end = std::chrono::high_resolution_clock::now();
    auto power_law_7_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // 結果の出力
    std::cout << "=== ベンチマーク結果 ===\n\n";
    
    print_benchmark_results("Legacy手法（基準）", legacy_duration, legacy_result, legacy_result, legacy_duration);
    print_benchmark_results("直接手法", direct_duration, direct_result, legacy_result, legacy_duration);
    print_benchmark_results("OpenMP並列処理版", openmp_duration, openmp_result, legacy_result, legacy_duration);
    print_benchmark_results("SIMD並列処理版", simd_duration, simd_result, legacy_result, legacy_duration);
    print_benchmark_results("Power Law Tool 7版", power_law_7_duration, power_law_7_result, legacy_result, legacy_duration);

    // 結果の一致確認
    const double epsilon = 1e-10;
    bool results_match = 
        std::abs(direct_result - openmp_result) < epsilon &&
        std::abs(direct_result - simd_result) < epsilon &&
        std::abs(direct_result - power_law_7_result) < epsilon;

    std::cout << "新手法の結果一致: " << (results_match ? "すべての実装が同じ結果を返しています" : "警告：実装間で結果が異なります") << "\n";
}

int main() {
    std::cout << "パワーロー分布に従うデータを生成中...\n";
    
    std::vector<int> test_sizes = {20,10000000, 100000000, 200000000};
    double exceeded_waiting_time = 0.0;  // 初期値
    
    for (int size : test_sizes) {
        std::cout << "\n=== テストケース: " << size << "点 ===\n";
        auto result = generate_power_law_point_process(1.5, 1.0, size, exceeded_waiting_time);
        std::vector<double> test_data = result.first;
        exceeded_waiting_time = result.second;  // 次のイテレーションのために保存
        run_benchmark(test_data);
    }
    
    return 0;
}
