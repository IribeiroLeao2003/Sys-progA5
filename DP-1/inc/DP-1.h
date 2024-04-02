#include "../../common/inc/common.h"

#define kSharedMIDBuffer    10 
#define kOneMileSecond      1000
#define kSecondsToSleep     2
#define kPathtoDP2exe       "../DP-2/bin"

char getRandomLetter();
int writeToBuffer(SharedMemory* shmPtr, int semId);