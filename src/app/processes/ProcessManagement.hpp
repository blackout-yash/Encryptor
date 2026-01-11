#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include <queue>
#include <memory>
#include <semaphore.h>

#include "Task.hpp"

class ProcessManagement {
    public:
        ProcessManagement();
        ~ProcessManagement();
        bool submitToQueue(std::unique_ptr <Task> task);
        void executeTask();

    private:
        struct SharedMemory {
            std::atomic <int> size;
            char tasks[1000][256];
            int front;
            int rear;
        };

        int shmFd;
        SharedMemory *sharedMemory;
        const char *SHM_NAME = "/my_queue";

        std::mutex queueLock;
        sem_t *itemsSemaphore;
        sem_t *emptySlotsSemaphore;
};

#endif