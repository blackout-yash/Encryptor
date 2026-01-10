#include "Task.hpp"
#include <iostream>
#include <filesystem>
#include "ProcessManagement.hpp"

int main(int argc, char *argv[]) {
    std::string strAction;
    std::string directory;

    std::cout << "Enter the Directory Path: ";
    std::getline(std::cin, directory);

    std::cout << "Enter the action(Encrypt/Decrypt): ";
    std::getline(std::cin, strAction);

    try {
        if(std::filesystem::exists(directory) && std::filesystem::is_directory(directory)) {
            ProcessManagement processManagement;
            for(const auto &entry: std::filesystem::recursive_directory_iterator(directory)) {
                if(entry.is_regular_file()) {
                    std::string filePath = entry.path().string();

                    IO io(filePath);
                    std::fstream fs = std::move(io.getFileStream());

                    if(fs.is_open()) {
                        Action action = (strAction == "Encrypt" ? Action::ENCRYPT : Action::DECRYPT);
                        auto task = std::make_unique <Task> (action, std::move(fs), filePath);
                        processManagement.submitToQueue(std::move(task));
                    } else {
                        std::cout << "Error::Unable to open file: " << filePath << std::endl;
                    }
                }
            }
            processManagement.executeTask();
        } else {
            std::cout << "Error::Invalid directory path: " << directory << std::endl;
        }
    } catch (const std::filesystem::filesystem_error &er) {
        std::cout << "Error::Filesystem Error: " << er.what() << std::endl;
    }

    return 0;
}