#pragma once

#include <string>
#include <random>

class SaltGenerator final {
public:
    static std::string generateSalt(int length);

private:
    inline static int RANDOM_INT_MIN = 0, RANDOM_INT_MAX = 255;
    inline static std::string CHARSET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
};