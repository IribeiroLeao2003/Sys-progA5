/*
* File: DC.c
* Project: SENG2030-24W - A-05 The Hisogram System
* Programmer: Vivian Morton, Isaac Ribeiro Leao, Jakob Warywoda 
* First Version: 01-04-2024
* Description: This file constants and function prototypes for the DC
*/

#include "../../common/inc/common.h"

//CMD Args
#define kSharedMemLoc      0
#define kDP1ProcIDLoc      1
#define kDP2ProcIDLoc      2
#define kArgsCount         3

#define kSleepTime         2



//function prototypes
int attachToResources(SharedMemory* pSharedMem, int sharedMemId, int* semId);
void processLoop(int sharedMemId, pid_t DP1PID, pid_t DP2PID);
int closeSharedMem(int sharedMemID);