#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "power_law_tool.hpp"

using namespace std;


int main() {
    string output_file = "power_law_process_test.csv";
    double alpha = 1.5; //べき指数
    double tau_0 = 1.0; //スケール因子
    int sample_amount = pow(10,7); //サンプル数

    ofstream output(output_file);
    
    std::vector<double> point_process = generate_power_law_point_process(alpha, tau_0, sample_amount);
    
    for (int i = 0; i < sample_amount; ++i){
        output << i << "," << point_process[i] << endl;
    }
        
    output.close();
    cout << "結果をファイル " << output_file << " に保存しました。" << endl;
}
