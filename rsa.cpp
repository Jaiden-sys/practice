// rsa.cpp
#include "rsa.h"
#include "rsamath.h"


RSA::RSA() : p(0), q(0), n(0), phi(0), e(0), d(0), block_size(0) {}

bool RSA::isCoprime(uint64_t a, uint64_t b) const {
    return std::gcd(a, b) == 1; //gcd же работает только дл€ маленьких чисел uint64_t, если он будет использоватьс€ с большими числами, то
}                               //надо переписать

void RSA::generatePrimes() {
    p = math::generatePrime(MIN, MAX);
    do {
        q = math::generatePrime(MIN, MAX);
    } while (q == p); // √арантируем разные простые числа
}

void RSA::calcN() {
    n = p * q;
}

void RSA::calcEuler() {
    phi = (p - 1) * (q - 1);
}

void RSA::calcPublicExponent() {
    const std::vector<uint64_t> candidates = { 65537, 257, 17, 5 };
    for (uint64_t candidate : candidates) {
        if (candidate < phi && isCoprime(candidate, phi)) {
            e = candidate;
            return;
        }
    }
    while (!isCoprime(e, phi)) {
        e = math::random(17, phi - 1);
    }
}

void RSA::calcSecretExponent() {
    int64_t x, y;
    uint64_t g = math::extended_gcd(e, phi, x, y);
    if (g != 1) throw std::runtime_error("No modular inverse");

    d = static_cast<uint64_t>((x % static_cast<int64_t>(phi) + static_cast<int64_t>(phi)) % static_cast<int64_t>(phi));

    // ќтладка:
    std::cout << "Check (e*d mod phi) = " << math::mulmod(e, d, phi) << '\n';
}


void RSA::read_input_file(const std::string& filename) {
    std::cout << "Trying to open: " << filename << '\n';
    std::cout << "Working directory: " << std::filesystem::current_path() << '\n';
    std::ifstream in(filename);
    if (!in) { throw std::runtime_error("Cannot open file: " + filename); }
    std::stringstream buffer;
    buffer << in.rdbuf();
    RSA::input_text = buffer.str();
}

void RSA::write_encoded_text(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) { throw std::runtime_error("Cannot create file: " + filename); }
    for (uint64_t block : encrypted_message) {
        out << block << '\n';
    }
    out.close();
}

void RSA::write_decoded_text(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) { throw std::runtime_error("Cannot create file: " + filename); }

    out << BlocksToString(decrypted_message);
}

uint16_t RSA::evalBlockSize() const {
    uint16_t k = 0;
    uint64_t max_val = 1;
    // ¬ычисл€ем максимальное число, которое помещаетс€ в n
    while (max_val <= n / 256) {
        max_val *= 256;
        k++;
    }
    return k;
}

void RSA::StringToBlocks() {
    block_size = evalBlockSize();
    message.clear();

    // ƒобавьте проверку
    if (block_size == 0) {
        throw std::runtime_error("Block size cannot be zero");
    }

    for (size_t i = 0; i < input_text.size(); i += block_size) {
        uint64_t block = 0;
        size_t max_j = std::min(static_cast<size_t>(block_size), input_text.size() - i);

        // »справленный пор€док байтов
        for (size_t j = 0; j < max_j; ++j) {
            block <<= 8;
            block |= static_cast<unsigned char>(input_text[i + j]);
        }
        // ƒополнение последнего блока нул€ми справа
        if (max_j < block_size) {
            block <<= 8 * (block_size - max_j);
        }
        message.push_back(block);
    }
}


std::string RSA::BlocksToString(const std::vector<uint64_t>& blocks) const {
    std::string result;
    for (uint64_t block : blocks) {
        for (int i = block_size - 1; i >= 0; --i) {
            char byte = static_cast<char>((block >> (8 * i)) & 0xFF);
            result.push_back(byte);
        }
    }
    return result.substr(0, input_text.size());
}


void RSA::encrypt() {
    encrypted_message.clear();
    for (auto m : message) {
        if (m >= n) {
            throw std::runtime_error("Message block too large");
        }
        encrypted_message.push_back(math::modPow(m, e, n));
    }

}

void RSA::decrypt() {
    uint64_t dp = d % (p - 1);
    uint64_t dq = d % (q - 1);

    int64_t x = 0, y = 0;
    int64_t a = static_cast<int64_t>(q);
    int64_t b = static_cast<int64_t>(p);

    int64_t g = math::extended_gcd(a, b, x, y);

    if (g != 1) {
        throw std::runtime_error("Cannot compute modular inverse of q mod p");
    }
    uint64_t q_inv = static_cast<uint64_t>((x % p + p) % p);

    decrypted_message.clear();

    for (uint64_t c : encrypted_message) {
        uint64_t m1 = math::modPow(c, dp, p);
        uint64_t m2 = math::modPow(c, dq, q);
        uint64_t diff = (m1 >= m2) ? (m1 - m2) : (p + m1 - m2);
        uint64_t h = (q_inv * diff) % p;
        uint64_t m = m2 + h * q;
        decrypted_message.push_back(m);
    }
}

