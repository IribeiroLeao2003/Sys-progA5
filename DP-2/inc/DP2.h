#include "../../common/inc/common.h"

#define kPathToDCexe    "../../DC/bin/DC"
#define kDCProcessName  "DC"
#define kPathCommonBin  "../../common/bin"

#define kshmIDBufferSize    10
#define kPIDSize            10


// void spawnDC(int* shmID, pid_t DP1_pid, pid_t DP2_pid);
void spawnDC(int* shmID, pid_t DP1_pid);
char getRandomLetter();
int getSemaphoreInfo(int* semaphoreID, key_t* semaphoreKey);