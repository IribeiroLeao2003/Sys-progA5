/*
* File: DC.c
* Project: SENG2030-24W - A-05 The Hisogram System
* Programmer: Vivian Morton, Isaac Ribeiro Leao, Jakob Warywoda 
* First Version: 01-04-2024
* Description: This file contains constants and function prototypes for the DC
*/

#include "../../common/inc/common.h"
#include <time.h>

//CMD Args
#define kSharedMemLoc      0
#define kDP1ProcIDLoc      1
#define kDP2ProcIDLoc      2
#define kArgsCount         3
#define kReadCount         20

#define kSleepTime         2
#define kTenSeconds        10

//function prototypes
void shutDownHandler(int SignalNumber);
int attachToResources(SharedMemory* pSharedMem, int sharedMemId, int* semId);
void processLoop(SharedMemory* pSharedMem, int semId);
int closeSharedMem(int sharedMemID);
int closeSemaphore(int semaphoreId);
void printData();