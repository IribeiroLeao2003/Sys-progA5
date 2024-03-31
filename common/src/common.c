#include "common.h"




int handleSharedMemory(key_t key) {


    int shm_id = shmget(key, kSharedMemorySize, 0644); // atemp to create shared memory

    // if doesnt already exist simply create one
    if (shm_id < 0) {
       
        shm_id = shmget(key, kSharedMemorySize, 0644 | IPC_CREAT);
        if (shm_id < 0) {
            perror("shmget");
            exit(kError);
        }
        printf("Shared memory succefully created\n");
    } else {
        printf("Shared memory was already created\n");
    }
    return shm_id;
}