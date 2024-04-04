#include "../../common/inc/common.h"

#define kSharedMIDBuffer    10 
#define kOneMileSecond      1000
#define kSecondsToSleep     2
#define k0String            "0"
#define kPathtoDP2          "../../DP-2/bin/DP2"
#define kPathtoDC           "../../DC/bin/DC"

char getRandomLetter();
int writeToBuffer(SharedMemory* shmPtr, int semId);
void launchChildDP2(int smID);