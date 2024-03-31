#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>

#define kSharedMemorySize   258
#define kSemKey             0x3214
#define kBufferSize         256
#define kSuccess            0 
#define kError              -1


struct kSharedMemory {
    char buffer[kBufferSize];
    int read_index;
    int write_index;
};

int handleSharedMemory(key_t key);