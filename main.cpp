#include <iostream>
#include "rsa.h"
#include "rsamath.h"
//Стандарт C++ 20

int main() {
    try {
        RSA rsa;
        std::cout << "=== RSA DEBUG MODE ===\n";

        // 1. Чтение входных данных
        std::cout << "\n[1] Reading input text...\n";
        rsa.read_input_file("input.txt");
        std::cout << "File size: " << rsa.BlocksToString({}).size() << " bytes\n";

        // 2. Генерация ключей с проверкой
        std::cout << "\n[2] Generating keys:\n";
        rsa.generatePrimes();
        rsa.calcN();
        rsa.calcEuler();
        rsa.calcPublicExponent();
        rsa.calcSecretExponent();

        // Вывод параметров ключей
        std::cout << "p = " << rsa.getP() << " ("
            << (math::isPrimeMillerRabin(rsa.getP()) ? "prime" : "NOT PRIME") << ")\n";
        std::cout << "q = " << rsa.getQ() << " ("
            << (math::isPrimeMillerRabin(rsa.getQ()) ? "prime" : "NOT PRIME") << ")\n";
        std::cout << "n = " << rsa.getN() << " (0x" << std::hex << rsa.getN() << std::dec << ")\n";
        std::cout << "phi = " << rsa.getEuler() << "\n";
        std::cout << "e = " << rsa.getPublicExponent() << "\n";
        std::cout << "d = " << rsa.getSecretExponent() << "\n";

        // Проверка: e*d mod phi должно быть 1
        const uint64_t check = (rsa.getPublicExponent() * rsa.getSecretExponent()) % rsa.getEuler();
        std::cout << "e*d mod phi = " << check << (check == 1 ? " (OK)" : " (ERROR!)") << "\n";

        // 3. Тест модульной арифметики
        std::cout << "\n[3] Modular arithmetic test:\n";
        uint64_t test_num = 123456;
        uint64_t encrypted_test = math::modPow(test_num, rsa.getPublicExponent(), rsa.getN());
        uint64_t decrypted_test = math::modPow(encrypted_test, rsa.getSecretExponent(), rsa.getN());
        std::cout << "Test number: " << test_num
            << "\nEncrypted: " << encrypted_test
            << "\nDecrypted: " << decrypted_test
            << " -> " << (test_num == decrypted_test ? "SUCCESS" : "FAILURE") << "\n";

        // 4. Преобразование в блоки
        std::cout << "\n[4] String to blocks conversion:\n";
        rsa.StringToBlocks();
        auto blocks = rsa.getMessage();
        std::cout << "Block size: " << rsa.evalBlockSize() << " bytes\n";
        std::cout << "Blocks count: " << blocks.size() << "\n";

        // Проверка первого блока
        if (!blocks.empty()) {
            std::cout << "First block value: " << blocks[0]
                << " (hex: 0x" << std::hex << blocks[0] << std::dec << ")\n";
            std::cout << "n = " << rsa.getN()
                << " -> block " << (blocks[0] < rsa.getN() ? "< n (OK)" : ">= n (OVERFLOW!)") << "\n";
        }

        // 5. Шифрование
        std::cout << "\n[5] Encryption:\n";
        rsa.encrypt();
        auto encrypted = rsa.getEncrypted();
        std::cout << "Encrypted blocks: " << encrypted.size() << "\n";
        if (!encrypted.empty()) {
            std::cout << "First encrypted block: " << encrypted[0] << "\n";
        }

        // 6. Дешифровка
        std::cout << "\n[6] Decryption:\n";
        rsa.decrypt();
        auto decrypted = rsa.getDecrypted();
        std::cout << "Decrypted blocks: " << decrypted.size() << "\n";
        if (!decrypted.empty()) {
            std::cout << "First decrypted block: " << decrypted[0]
                << (blocks[0] == decrypted[0] ? " (OK)" : " (MISMATCH!)") << "\n";
        }

        // 7. Преобразование обратно в строку
        std::cout << "\n[7] Blocks to string:\n";
        std::string original = rsa.BlocksToString(blocks);
        std::string recovered = rsa.BlocksToString(decrypted);

        std::cout << "Original length: " << original.length() << "\n";
        std::cout << "Recovered length: " << recovered.length() << "\n";

        // Поиск различий
        bool match = true;
        for (size_t i = 0; i < std::min(original.size(), recovered.size()); ++i) {
            if (original[i] != recovered[i]) {
                std::cout << "Mismatch at position " << i
                    << ": '" << original[i] << "' (0x" << std::hex << (int)(unsigned char)original[i]
                    << ") vs '" << recovered[i] << "' (0x" << (int)(unsigned char)recovered[i] << ")\n";
                match = false;
                break;
            }
        }

        if (original == recovered) {
            std::cout << "FINAL RESULT: PERFECT MATCH!\n";
        }
        else {
            std::cout << "FINAL RESULT: DECRYPTION FAILED!\n";

            // Сохраняем для анализа
            std::ofstream("original.bin", std::ios::binary).write(original.data(), original.size());
            std::ofstream("recovered.bin", std::ios::binary).write(recovered.data(), recovered.size());
            std::cout << "Saved original and recovered files for comparison\n";
        }

        // Сохранение результатов
        rsa.write_encoded_text("cipher.txt");
        rsa.write_decoded_text("decrypted.txt");

    }
    catch (const std::exception& ex) {
        std::cerr << "\n!!! CRITICAL ERROR: " << ex.what() << " !!!\n";
        return 1;
    }
    return 0;
}
