#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <cmath>
#include <limits>
#include <algorithm>
#include <tuple>

using namespace std;

// Find the best fit linear regression model using the least squares method. Returns a tuple of slope, intercept, error.
tuple<double, double, double> find_best_fit(const vector<double>& y) {
    int n = y.size();
    vector<int> x(n);
    iota(x.begin(), x.end(), 1);  // x = 1, 2, ..., n

    // average of x
    double x_mean = accumulate(x.begin(), x.end(), 0.0) / n;
    // average of y
    double y_mean = accumulate(y.begin(), y.end(), 0.0) / n;

    // Calculate numerator and denominator
    double num = 0.0;
    double den = 0.0;
    for (int i = 0; i < n; ++i) {
        num += (x[i] - x_mean) * (y[i] - y_mean);
        den += (x[i] - x_mean) * (x[i] - x_mean);
    }

    // Calculate slope and intercept
    double slope = num / den;
    double intercept = y_mean - slope * x_mean;

    // Calculate residual sum of squares
    double error = 0.0;
    for (int i = 0; i < n; ++i) {
        double predicted = slope * x[i] + intercept;
        double residual = y[i] - predicted;
        error += residual * residual;
    }

    return make_tuple(slope, intercept, error);
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

int main() {
    string base_name = "beki_walk_alpha_0.1";
    string input_path = base_name + ".csv";
    string output_path = "l_F_" + base_name + ".csv";

    int first_l = 4;
    int last_l = 1000;

    ifstream file(input_path);
    vector<double> data;
    string line, item;


    // Read data from CSV
    while (getline(file, line)) {
        stringstream ss(line);
        while (getline(ss, item, ',')) {
            data.push_back(stod(item));
        }
    }
    file.close();
    cout << "checkpoint 1: CSV data reading complete" << endl; // checkpoint
    
 
    // Output 10 data contents
    for (int i = 0; i < 10; ++i) {
        cout << "data" << data[i] << endl;
    }
    cout << "checkpoint 2: Output 10 data contents complete" << endl;

    // Adjust data
    double start_point = data.front();
    transform(data.begin(), data.end(), data.begin(), [start_point](double y) { return y - start_point; });

    int N = data.size();
    vector<pair<int, double>> records_l_F;
    int l;
    for (l = first_l ; l < last_l ; ++l) {
        if (N % l > 50) continue;

        int num_segments = N / l;
        int N_used = num_segments * l; // Number of data to use
        vector<vector<double>> segments = generate_segments(data, l);
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
    for (const pair<int, double>& record : records_l_F) {
        cout << "l: " << record.first << ", F: " << record.second << endl;
    }

    ofstream record_file(output_path);
    record_file << "l,F\n";
    for (const pair<int, double>& record : records_l_F) {
        record_file << record.first << "," << record.second << "\n";
    }
    record_file.close();

    return 0;
}
