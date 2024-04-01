/*
* File: common.h
* Project: SENG2030-24W - A-05 The Hisogram System
* Programmer: Vivian Morton, Isaac Ribeiro Leao, Jakob Warywoda 
* First Version: 01-04-2024
* Description: This file constants common structs, constants, and function prototypes for the system to use
*/

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
#define kSingleUseSemaphore 0


typedef struct{
    char buffer[kBufferSize];
    int readIndex;
    int writeIndex;
}SharedMemory;

//Semaphore control structs
struct sembuf getSem = { 0, -1, SEM_UNDO };
struct sembuf releaseSem = { 0, 1, SEM_UNDO };

int initSharedMem(int* sharedMemID, key_t* sharedMemKey);
int createSemaphore(int* semaphoreID, key_t* semaphoreKey);
int attachSemaphore(int* semaphoreID);