
#pragma once
#include <random>
#include <vector>
#include <cstdint>
#include <intrin.h>
namespace math {
    static const std::vector<std::pair<uint64_t, std::vector<uint64_t>>> WITNESS_RANGES = {
        {2047, {2}},
        {1373653, {2, 3}},
        {9080191, {31, 73}},
        {25326001, {2, 3, 5}},
        {3215031751, {2, 3, 5, 7}},
        {4759123141, {2, 7, 61}},
        {1122004669633, {2, 13, 23, 1662803}},
        {2152302898747, {2, 3, 5, 7, 11}},
        {3474749660383, {2, 3, 5, 7, 11, 13}},
        {341550071728321, {2, 3, 5, 7, 11, 13, 17}}
    };

    static constexpr uint64_t MIN = 100000;
    static constexpr uint64_t MAX = 200000;
    bool isPrimeMillerRabin(uint64_t n);
    uint64_t generatePrime(uint64_t min, uint64_t max);
    uint64_t modPow(uint64_t base, uint64_t exp, uint64_t mod);
    uint64_t random(uint64_t min, uint64_t max);
    uint64_t extended_gcd(uint64_t a, uint64_t b, int64_t& x, int64_t& y);
    uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod);
};