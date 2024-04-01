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
#define kCheckExists        0
#define kSemaphoreID        87996 //based on Isaacs student ID
#define kZeroFlag           0


typedef struct{
    char buffer[kBufferSize];
    int readIndex;
    int writeIndex;
}SharedMemory;

int handleSharedMemory(key_t key);