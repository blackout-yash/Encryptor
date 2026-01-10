#include <iostream>
#include <sys/wait.h>
#include "Cryption.hpp"
#include "ProcessManagement.hpp"

ProcessManagement::ProcessManagement() {}

bool ProcessManagement::submitToQueue(std::unique_ptr <Task> task) {
    taskQueue.push(std::move(task));
    return true;
}

void ProcessManagement::executeTask() {
    while(!taskQueue.empty()) {
        std::unique_ptr <Task> taskToExecute = std::move(taskQueue.front());
        taskQueue.pop();

        std::cout << "Executing Task: " << taskToExecute -> toString() << std::endl;
        executeCryption(taskToExecute -> toString());
    }
}