#include "common.h"

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


int createSemaphore(int* semaphoreID, key_t* semaphoreKey)
{
    int errorStatus = kSuccess;

    //generate key
    *semaphoreKey = ftok("../../common/bin", kSemaphoreID); //common/bin directory

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
