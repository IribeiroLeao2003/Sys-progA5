#include "../../common/inc/common.h"

//CMD Args
#define kSharedMemLoc      0
#define kDP1ProcIDLoc      1
#define kDP2ProcIDLoc      2
#define kArgsCount         3

#define kSleepTime         2



//function prototypes
void processLoop(int sharedMemId, pid_t DP1PID, pid_t DP2PID);
int closeSharedMem(int sharedMemID);