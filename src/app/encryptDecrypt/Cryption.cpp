#include "Task.hpp"
#include "ReadEnv.cpp"
#include "Cryption.hpp"

int executeCryption(const std::string &taskData) {
    Task task = Task::fromString(taskData);

    ReadEnv env;
    std::string envKey = env.getEnv();
    int key = stoi(envKey);

    if(task.action == Action::ENCRYPT) {
        char ch;
        while(task.fs.get(ch)) {
            ch = (ch + key) % 256;
            task.fs.seekp(-1, std::ios::cur);
            task.fs.put(ch);
        }
        task.fs.close();
    } else {
        char ch;
        while(task.fs.get(ch)) {
            ch = (ch - key + 256) % 256;
            task.fs.seekp(-1, std::ios::cur);
            task.fs.put(ch);
        }
        task.fs.close();        
    }
    return 0;
}