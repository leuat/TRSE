
#ifndef RANDOM_H
#define RANDOM_H
#include <random>
#include <ctime>

#ifdef QT_VERSION
    #include <QVector3D>
#endif

#if defined(__clang__)
// thread_local is not supported in clang so this is not thread safe. Please use GCC
static std::mt19937 generator;
#else
// this generator is now thread safe so we can use it with OpenMP.
static thread_local std::mt19937 generator;
#endif

class Random
{
public:
    static void seed(uint64_t seed = std::mt19937_64::default_seed) {
        generator.seed(seed);
    }
    static void randomSeed() {
        generator.seed(std::time(0));
    }

    static float nextFloat(const float & min, const float & max) {
        // Random float between min and max
        std::uniform_real_distribution<float> distribution(min,max);
        return distribution(generator);
    }
    static float nextFloat() {
        // Random float between 0 and 1
        std::uniform_real_distribution<float> distribution(0,1);
        return distribution(generator);
    }
    static double nextDouble(const double & min, const double & max) {
        // Random double between min and max
        std::uniform_real_distribution<double> distribution(min,max);
        return distribution(generator);
    }
    static double nextDouble() {
        // Random double between 0 and 1
        std::uniform_real_distribution<double> distribution(0,1);
        return distribution(generator);
    }
    static double nextGaussian(const double & mean, const double & sigma) {
        // Normally distributed random number.
        std::normal_distribution<double> distribution(mean, sigma);
        return distribution(generator);
    }
    static int nextInt(const int & min, const int & max) {
        // Random int between min and max
        std::uniform_int_distribution<int> distribution(min,max);
        return distribution(generator);
    }
    static long nextLong(const long & min, const long & max) {
        // Random long between min and max
        std::uniform_int_distribution<long> distribution(min,max);
        return distribution(generator);
    }
    static bool nextBool() {
        // Random bool
        std::uniform_int_distribution<int> distribution(0,1);
        return distribution(generator);
    }
#ifdef QT_VERSION
    static QVector3D nextQVector3D(float min, float max) {
        float x = nextFloat(min, max);
        float y = nextFloat(min, max);
        float z = nextFloat(min, max);
        return QVector3D(x,y,z);
    }
#endif
};

#endif // RANDOM_H
