#include "../../common/inc/common.h"

#define kPathToDCexe    "../../DC/bin/dc"
#define kDCProcessName  "dc"

#define kshmIDBufferSize    10
#define kPIDSize            10


// void spawnDC(int* shmID, pid_t DP1_pid, pid_t DP2_pid);
void spawnDC(int* shmID, pid_t DP1_pid);