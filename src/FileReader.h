#pragma once

#include "e_type.h"

#include <string>
#include <fstream>
#include <vector>

class FileReader final {
public:
    static inline std::string read(const std::string& filename) {
        std::string buffer;
        std::ifstream fileStream(filename, std::ios::in);
        if (fileStream.is_open()) {
            buffer.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());
            // fileStream.seekg(0, std::ios::end);
            // size_t size = fileStream.tellg();
            // buffer.resize(size + 1, '\0');
            // fileStream.seekg(0);
            // fileStream.read(&buffer[0], size);
            fileStream.close();
        }
        return buffer;
    }
};