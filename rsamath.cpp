
#include "rsamath.h"
#include <random>
#include <cmath>
#include <cstdint>
namespace math {

    uint64_t modPow(uint64_t base, uint64_t exp, uint64_t mod) {
        uint64_t result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp & 1) {

                result = mulmod(result, base, mod);
            }
            base = mulmod(base, base, mod);
            exp >>= 1;
        }
        return result;
    }
    uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod) { // Сравнение по модулю в некоторой степени
        uint64_t res;
        const uint64_t q = (uint64_t)((double)a * (double)b / mod);
        res = a * b - q * mod;
        return res >= mod ? res - mod : res;
    }
    bool isPrimeMillerRabin(uint64_t n) { //вероятностный тест Миллера-Рабина на простоту
        for (uint64_t p : {2, 3, 5, 6, 11, 13, 17, 19, 23, 29}) {
            if (n % p == 0) { return n == p; }
        }
        if ((n & 1) == 0) return false;

        uint64_t d = n - 1;
        uint64_t s = 0;
        while ((d & 1) == 0) {
            d >>= 1;
            s++;
        }
        for (const auto& [limit, bases] : WITNESS_RANGES) {
            if (n < limit) {
                for (uint64_t a : bases) {
                    uint64_t x = modPow(a, d, n);
                    if (x == 1 || x == n - 1) continue;
                    bool composite = true;
                    for (uint64_t j = 0; j < s - 1; j++) {
                        x = modPow(x, 2, n);
                        if (x == n - 1) {
                            composite = false;
                            break;
                        }
                    }
                    if (composite) return false;
                }
                return true;
            }
        }
        return true;
    }
    std::mt19937& get_rn() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }
    uint64_t random(uint64_t min, uint64_t max) { //"настоящий" рандом через mt19937
        std::uniform_int_distribution<uint64_t> distib(min, max);
        return distib(get_rn());
    }
    uint64_t generatePrime(uint64_t min, uint64_t max) {
        while (true) {
            uint64_t candidate = random(min, max);
            if (isPrimeMillerRabin(candidate)) return candidate;
        }
    }
    uint64_t extended_gcd(uint64_t a, uint64_t b, int64_t& x, int64_t& y) { //Расширенный алгоритм Евклида
        if (a == 0) {
            x = 0;
            y = 1;
            return b;
        }
        int64_t x1 = 0, y1 = 0;  // signed!
        uint64_t d = extended_gcd(b % a, a, x1, y1);
        x = y1 - static_cast<int64_t>(b / a) * x1;
        y = x1;
        return d;
    }


};

