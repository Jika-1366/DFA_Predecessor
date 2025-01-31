//ウォークを生成し、CSVファイルに保存するプログラム

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include "power_law_tool.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    // デフォルト値の設定
    string output_file = "walk_data.csv";
    double tau_0 = 1.0;
    int sample_amount = pow(10,6);
    double alpha = 1.0;

    // コマンドライン引数の解析
    for (int i = 1; i < argc; i += 2) {
        string arg = argv[i];
        if (i + 1 < argc) {
            if (arg == "--output") output_file = argv[i+1];
            else if (arg == "--tau_0") tau_0 = stod(argv[i+1]);
            else if (arg == "--sample_amount") sample_amount = stoi(argv[i+1]);
            else if (arg == "--alpha") alpha = stod(argv[i+1]);
        }
    }

    cout << "設定値:" << endl;
    cout << "output_file = " << output_file << endl;
    cout << "tau_0 = " << tau_0 << endl;
    cout << "sample_amount = " << sample_amount << endl;
    cout << "alpha = " << alpha << endl;

    ofstream output(output_file);

    std::vector<double> walk = generate_power_law_point_process(alpha, tau_0, sample_amount);

    for (const auto& value : walk) {
        output << setprecision(15) << value << endl;
    }

    output.close();
    cout << "ウォークデータをファイル " << output_file << " に保存しました。" << endl;
}