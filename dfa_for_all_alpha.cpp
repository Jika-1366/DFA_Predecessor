#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "power_law_tool.hpp"

using namespace std;

void process_data(const string& input_path) {
    ifstream file(input_path);
    vector<double> data;
    string line, item;

    // CSVからデータを読み込む
    while (getline(file, line)) {
        stringstream ss(line);
        while (getline(ss, item, ',')) {
            data.push_back(stod(item));
        }
    }
    file.close();
    cout << "checkpoint 1: CSVデータの読み込み完了" << endl; // チェックポイント

    // データの内容を10個出力
    for (int i = 0; i < 10; ++i) {
        cout << "data" << data[i] << endl;
    }
    cout << "checkpoint 2: データの内容を10個出力完了" << endl;

    double slope = dfa(data);
    cout << "slope: " << slope << endl;
}

int main() {
    string output_file = "alpha_slope.csv";
    ofstream output(output_file);
    
    output << "alpha,average_slope" << endl;

    const int num_iterations = 30; // 各alphaに対して生成するwalkの数
    
    for (double alpha = 0.1; alpha <= 2.9; alpha += 0.2) {
        vector<double> slopes;
        double tau_0 = 10.0;
        
        for (int i = 0; i < num_iterations; ++i) {
            std::vector<double> walk = generate_power_law_walk(alpha, tau_0 );
            double slope = dfa(walk);
            slopes.push_back(slope);
        }
        
        // 平均を計算（修正版）
        double average_slope = accumulate(slopes.begin(), slopes.end(), 0.0) / num_iterations;

        output << alpha << "," << average_slope << endl;
        
        cout << "Processed alpha = " << alpha << endl;
        cout << "then slope=  "<< average_slope << endl;  // 進捗を表示
    }

    output.close();
    cout << "結果をファイル " << output_file << " に保存しました。" << endl;
}
