#include "salt-generator.h"

std::string SaltGenerator::generateSalt(int length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(RANDOM_INT_MIN, RANDOM_INT_MAX);
    std::string salt;
    salt.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        salt += CHARSET[dist(gen) % CHARSET.size()];
    }

    return salt;
}