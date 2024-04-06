/*
* File: DC-Resources.c
* Project: SENG2030-24W - A-05 The Hisogram System
* Programmer: Vivian Morton, Isaac Ribeiro Leao, Jakob Warywoda 
* First Version: 01-04-2024
* Description: This file contains the logic for handling resources such as shared memory and the semaphore
*/

#include "../inc/DC.h"

/*
* FUNCTION    : attachToResources()
* DESCRIPTION : Attaches to the shared memory and semaphore for use later
* PARAMETERS  : SharedMemory* pSharedMem: pointer to the shared memory
*             : int sharedMemId: the shared memory Id
*             : int* semId: pointer to store the id of the semaphore
* RETURNS     : The error status of the proccess
*/
int attachToResources(SharedMemory** pSharedMem, int sharedMemId, int* pSemId)
{
    int errorStatus = kSuccess;
    while (*pSharedMem == NULL) //try to attack to shared Memory
    {
        *pSharedMem = (SharedMemory*) shmat(sharedMemId, NULL, kZeroFlag); //attach to memory if valid
        if (pSharedMem == NULL)
        {
            sleep(kSleepTime);
        }
    }

    //Get semaphore
    attachSemaphore(pSemId);
    //printf("Attached to semaphore in DC - <%d>\n", *pSemId);
    if (*pSemId == kError)
    {
        errorStatus = kError;
        perror(strerror(errno));
    }

    return errorStatus;
}


/*
* FUNCTION    : closeSharedMem()
* DESCRIPTION : This function takes the shared memory id and closes it if possible
* PARAMETERS  : int sharedMemID: the id of the shared memory to close
* RETURNS     : The error status of the proccess
*/
int closeSharedMem(int sharedMemID)
{
    int errorStatus = kSuccess;
    
    if (shmctl(sharedMemID, IPC_RMID, NULL) == kError) //try to close the message queue
    {
        perror(strerror(errno)); //note any errors from closing
        errorStatus = kError;
    }

    return errorStatus;
}



/*
* FUNCTION    : closeSemaphore()
* DESCRIPTION : This function takes the semaphore id and releases it if possible
* PARAMETERS  : int semaphoreId: the id of the semaphore to releases
* RETURNS     : The error status of the proccess
*/
int closeSemaphore(int semaphoreId)
{
    int errorStatus = kSuccess;
    
    if (semctl (semaphoreId, 0, IPC_RMID, 0) == kError) //try to close semaphore
    {
        perror(strerror(errno)); //note any errors from closing
        errorStatus = kError;
    }

    return errorStatus;
}