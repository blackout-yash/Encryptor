#include <fcntl.h> 
#include <iostream>
#include <unistd.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>   
#include <sys/stat.h>  
#include <semaphore.h>

#include "Cryption.hpp"
#include "ProcessManagement.hpp"

ProcessManagement::ProcessManagement() {
    itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);
    emptySlotsSemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, 1000);

    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shmFd, sizeof(SharedMemory));
    sharedMemory = static_cast <SharedMemory*> (mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0));
    sharedMemory -> front = 0;
    sharedMemory -> rear = 0;
    sharedMemory -> size.store(0);
}

ProcessManagement::~ProcessManagement() {
    munmap(sharedMemory, sizeof(SharedMemory));
    shm_unlink(SHM_NAME);
}

bool ProcessManagement::submitToQueue(std::unique_ptr <Task> task) {
    sem_wait(emptySlotsSemaphore);
    std::unique_lock <std::mutex> lock(queueLock);

    if(sharedMemory -> size.load() >= 1000) {
        return false;
    }

    strcpy(sharedMemory -> tasks[sharedMemory -> rear], task -> toString().c_str());
    sharedMemory -> rear = (sharedMemory -> rear + 1) % 1000;
    sharedMemory -> size.fetch_add(1);

    lock.unlock();
    sem_post(itemsSemaphore);

    int pid = fork();
    if(pid < 0) {
        return false;
    } else if(pid > 0) {
        std::cout << "Entering the parent process." << std::endl;
    } else {
        std::cout << "Entering the child process." << std::endl;
        executeTask();
        exit(0);
        std::cout << "Exiting the child process." << std::endl;
    }

    return true;
}

void ProcessManagement::executeTask() {
    sem_wait(itemsSemaphore);
    std::unique_lock <std::mutex> lock(queueLock);

    char tastStr[256];
    strcpy(tastStr, sharedMemory -> tasks[sharedMemory -> front]);
    sharedMemory -> front = (sharedMemory -> front + 1) % 1000;
    sharedMemory -> size.fetch_sub(1);

    lock.unlock();
    sem_post(emptySlotsSemaphore);

    std::cout << "Executing Child Process" << std::endl;
    executeCryption(tastStr);
}