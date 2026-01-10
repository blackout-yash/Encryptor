#include "IO.hpp"
#include <sstream>

class ReadEnv {
    public:

    std::string getEnv() {
        std::string envPath = ".env";

        IO io(envPath);
        std::fstream fs = io.getFileStream();

        std::stringstream buffer;
        buffer << fs.rdbuf();

        std::string content = buffer.str();
        return content;
    }
};