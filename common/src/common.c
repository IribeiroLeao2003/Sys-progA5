/*
* File: common.c
* Project: SENG2030-24W - A-05 The Hisogram System
* Programmer: Vivian Morton, Isaac Ribeiro Leao, Jakob Warywoda 
* First Version: 01-04-2024
* Description: This file contains common functions for the system to use
*/

#include "../../common/inc/common.h"

struct sembuf getSem = { 0, -1, SEM_UNDO };
struct sembuf releaseSem = { 0, 1, SEM_UNDO };

/*
* FUNCTION    : initSharedMem()
* DESCRIPTION : This function takes a pointer to the shared memory ID and shared memory key to create new shared memory if possible
* PARAMETERS  : int* sharedMemID: A pointer to the shared memory ID so it can be modifed
*             : key_t sharedMemKey: the key for the shared memory to use
* RETURNS     : The error status of the proccess
*/
int initSharedMem(int* sharedMemID, key_t* sharedMemKey)
{

    int errorStatus = kSuccess;
    //Check if shared memory exists
    *sharedMemID = shmget(*sharedMemKey, sizeof(SharedMemory), kCheckExists);

    if (*sharedMemID == kError)
    {
        //does not exist so create it
        *sharedMemID = shmget(*sharedMemKey, sizeof(SharedMemory), (IPC_CREAT | 0660));
        if (*sharedMemID == kError) //check if it failed
        {
            perror("shmget");
            errorStatus = kError;
        }
    }

    return errorStatus;
}



/*
* FUNCTION    : createSemaphore()
* DESCRIPTION : This function takes a pointer to the semaphore ID and semaphore key to create new semaphore if possible
* PARAMETERS  : int* semaphoreID: A pointer to the semaphore ID so it can be modifed
*             : key_t semaphoreKey: the key for the semaphore to use
* RETURNS     : The error status of the proccess
*/
int createSemaphore(int* semaphoreID, key_t* semaphoreKey)
{
    int errorStatus = kSuccess;

    //generate key
    *semaphoreKey = ftok("../../common/bin", kSemaphoreID); //common/bin directory

    // check for errors
     if (*semaphoreKey == (key_t)kError) {
        perror("ftok failed for semaphore");
        return kError;
    }

    //check if semaphore exists
    if (*semaphoreKey != kError)
    {
        *semaphoreID = semget(*semaphoreKey, kSingleUseSemaphore, (IPC_CREAT | 0666));
        if (*semaphoreID == kError) //check if it failed
        {
            perror("shmget");
            errorStatus = kError;
        }
    }
    else
    {
        errorStatus = kError;
    }

    return errorStatus;
}




/*
* FUNCTION    : attachSemaphore()
* DESCRIPTION : This function takes a pointer to the semaphore ID and attached to a the semaphore if possible
* PARAMETERS  : int* semaphoreID: A pointer to the semaphore ID so it can be modifed
* RETURNS     : The error status of the proccess
*/
int attachSemaphore(int* semaphoreID)
{
    int errorStatus = kSuccess;
    key_t semKey = 0;
    //generate key
    semKey = ftok("../../common/bin", kSemaphoreID); //common/bin directory

    //check if semaphore exists
    if (semKey != kError)
    {
        *semaphoreID = semget(semKey, kSingleUseSemaphore, kCheckExists);
        if (*semaphoreID == kError) //check if it failed
        {
            perror("shmget");
            errorStatus = kError;
        }
    }
    else
    {
        errorStatus = kError;
    }

    return errorStatus;
}



/*
* FUNCTION    : useSemaphore()
* DESCRIPTION : This function takes the semaphore ID and marks it as in use or waits until the semaphore is free to do so
* PARAMETERS  : int semId: The semaphore id to use
* RETURNS     : The results of the function call
*/
int useSemaphore(int semId)
{
    return semop(semId, &getSem, 1);
}


/*
* FUNCTION    : useSemaphore()
* DESCRIPTION : This function takes the semaphore ID and frees it from use
* PARAMETERS  : int semId: The semaphore id to use
* RETURNS     : The results of the function call
*/
int releaseSemaphore(int semId)
{
    return semop(semId, &releaseSem, 1);
}



/*
* FUNCTION    : incrementIndex()
* DESCRIPTION : This function takes an int pointer and increments the value unless it's at the top in which case it wraps back to zero
* PARAMETERS  : int index*: the pointer to the int to increment
* RETURNS     : void
*/
void incrementIndex(int* index)
{
    if (*index == kBufferTop)
    {
        *index = 0;
    }
    else
    {
        *index++;
    }
}