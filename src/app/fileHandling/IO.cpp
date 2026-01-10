#include "IO.hpp"
#include <iostream>

IO::IO(const std::string &filePath) {
    fileStream.open(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if(fileStream.is_open() == false) {
        std::cout << "Error::Unable to open file: " << filePath << std::endl;
    }
}

IO::~IO() {
    if(fileStream.is_open()) {
        fileStream.close();
    }
} 

std::fstream IO::getFileStream() {
    return std::move(fileStream);
}