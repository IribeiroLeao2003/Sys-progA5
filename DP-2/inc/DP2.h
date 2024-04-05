#include "../../common/inc/common.h"

#define kPathToDCexe    "../../DC/bin/DC"
#define kDCProcessName  "DC"
#define kPathCommonBin  "../../common/bin"

#define kshmIDBufferSize    10
#define kPIDSize            10
#define kDontWrite          -2
#define kOneTwentieth       50000


// void spawnDC(int* shmID, pid_t DP1_pid, pid_t DP2_pid);
void sigintHandler(int sig);
void spawnDC(int shmID, pid_t DP1_pid);
int getSemaphoreInfo(int* semaphoreID, key_t* semaphoreKey);
int writeLetterToBuffer(SharedMemory* pSharedMemory, int semaphoreID);