
#pragma once
#include <cstdint>
#include <numeric>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <iostream>

class RSA {
private:
    static constexpr uint64_t MIN = 100000;
    static constexpr uint64_t MAX = 200000; 

    uint64_t p;
    uint64_t q;
    uint64_t n;
    uint64_t phi;
    uint64_t e;
    uint64_t d;
    uint16_t block_size;
    std::string input_text;
    std::string encrypted_text;
    std::vector <uint64_t> message;
    std::string output;
    std::vector <uint64_t> encrypted_message;
    std::vector <uint64_t> decrypted_message;

public:
    RSA();
    bool isCoprime(uint64_t a, uint64_t b) const;
    void generatePrimes();
    void calcN();
    void calcEuler();
    void calcPublicExponent();
    void calcSecretExponent();
    void read_input_file(const std::string& filename);
    void write_encoded_text(const std::string& filename) const;
    void write_decoded_text(const std::string& filename) const;
    void encrypt();
    void decrypt();
    void StringToBlocks();
    std::string BlocksToString(const std::vector<uint64_t>& message) const;
    uint16_t evalBlockSize() const;
    uint64_t getP() const { return p; }
    uint64_t getQ() const { return q; }
    uint64_t getN() const { return n; }
    std::vector <uint64_t> getMessage() const { return message; }
    std::vector <uint64_t> getDecrypted() const { return decrypted_message; }
    uint64_t getEuler() const { return phi; }
    uint64_t getPublicExponent() const { return e; }
    uint64_t getSecretExponent() const { return d; }
    std::vector<uint64_t> getEncrypted() const { return encrypted_message; }

};
