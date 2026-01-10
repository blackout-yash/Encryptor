#ifndef TASK_HPP
#define TASK_HPP

#include "IO.hpp"
#include <sstream>

enum class Action {
    ENCRYPT,
    DECRYPT
};

struct Task {
    Action action;
    std::fstream fs;
    std::string filePath;

    Task(Action action, std::fstream &&fs, std::string filePath):action(action), fs(std::move(fs)), filePath(filePath) {}

    std::string toString() {
        std::ostringstream oss;
        oss <<  (action == Action::ENCRYPT ? "ENCRYPT" : "DECRYPT") << "," << filePath;
        return oss.str();
    }

    static Task fromString(const std::string &taskData) {
        std::istringstream iss(taskData);
        std::string filePath;
        std::string actionStr;

        if(std::getline(iss, actionStr, ',') && std::getline(iss, filePath)) {
            Action action = (actionStr == "ENCRYPT" ? Action::ENCRYPT : Action::DECRYPT);

            IO io(filePath);
            std::fstream fs = std::move(io.getFileStream());
            if(fs.is_open()) {
                return Task(action, std::move(fs), filePath);
            } else {
                throw std::runtime_error("Error::Failed to open File: " + filePath);
            }
        } else {
            throw std::runtime_error("Error::Invalid Task Data Format");
        }
    }
};

#endif