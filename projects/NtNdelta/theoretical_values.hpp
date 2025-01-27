#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <optional>
#include <filesystem>
#include <iomanip>
#include <limits>

struct TheoreticalKey {
    double alpha;
    int sample_amount;
    std::string method_name;

    bool operator<(const TheoreticalKey& other) const {
        if (alpha != other.alpha) return alpha < other.alpha;
        if (sample_amount != other.sample_amount) return sample_amount < other.sample_amount;
        return method_name < other.method_name;
    }
};

class TheoreticalValueManager {
private:
    std::map<TheoreticalKey, double> cache;
    std::string filename;

public:
    TheoreticalValueManager(const std::string& fname = "theoretical_values.csv") : filename(fname) {
        loadFromFile();
    }

    std::optional<double> getValue(double alpha, int sample_amount, const std::string& method_name) {
        TheoreticalKey key{alpha, sample_amount, method_name};
        auto it = cache.find(key);
        if (it != cache.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    void setValue(double alpha, int sample_amount, const std::string& method_name, double value) {
        TheoreticalKey key{alpha, sample_amount, method_name};
        cache[key] = value;
        saveToFile();
    }

private:
    void loadFromFile() {
        if (!std::filesystem::exists(filename)) return;

        std::ifstream file(filename);
        std::string line;
        
        // ヘッダーをスキップ
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string alpha_str, sample_amount_str, method_name, value_str;

            std::getline(ss, alpha_str, ',');
            std::getline(ss, sample_amount_str, ',');
            std::getline(ss, method_name, ',');
            std::getline(ss, value_str, ',');

            double alpha = std::stod(alpha_str);
            int sample_amount = std::stoi(sample_amount_str);
            double value = std::stod(value_str);

            TheoreticalKey key{alpha, sample_amount, method_name};
            cache[key] = value;
        }
    }

    void saveToFile() {
        std::ofstream file(filename);
        // doubleの最大精度（通常は15-17桁）を設定
        file << std::setprecision(std::numeric_limits<double>::max_digits10);
        file << "alpha,sample_amount,method_name,value\n";
        
        for (const auto& [key, value] : cache) {
            file << std::scientific  // 科学表記を使用
                 << key.alpha << ","
                 << key.sample_amount << ","
                 << key.method_name << ","
                 << value << "\n";
        }
    }
}; 