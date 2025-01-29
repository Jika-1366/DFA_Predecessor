#ifndef OLD_RANDOM_GENERATOR_HPP
#define OLD_RANDOM_GENERATOR_HPP

#include <cstdlib>
#include <ctime>
#include <cmath>


//なぜ以下のような一様分布の乱数生成が必要かというと、u=0の確率があまりに高すぎるので、それを防ぐためです。でもちょっと遅いのでXorShiftの方がいいかも。
namespace old_random {
    // プログラム開始時に一度だけ実行される初期化
    struct RandomInitializer {
        RandomInitializer() {
            srand((unsigned int)time(NULL));
        }
    } random_initializer;

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
}

#endif 