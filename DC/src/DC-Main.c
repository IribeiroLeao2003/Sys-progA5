/*
* File: DC-Main.c
* Project: SENG2030-24W - A-05 The Hisogram System
* Programmer: Vivian Morton, Isaac Ribeiro Leao, Jakob Warywoda 
* First Version: 01-04-2024
* Description: This file constants the main logic for the DC
*/

#include "../inc/DC.h"

//Free semaphore
//semctl (semid, 0, IPC_RMID, 0); 



int main(int argc, char* argv[])
{
    if (argc == kArgsCount)
    {
        //copy args
        int sharedMemId = atoi(argv[kSharedMemLoc]);
        pid_t DP1PID = atoi(argv[kDP1ProcIDLoc]);
        pid_t DP2PID = atoi(argv[kDP2ProcIDLoc]);

        //start process loop
        processLoop(sharedMemId, DP1PID, DP2PID);

    }
    else
    {
        return kError;
    }
}



/*
* FUNCTION    : processLoop()
* DESCRIPTION : The main loop for processing the DC
* PARAMETERS  : int sharedMemID: the id of the shared memory to use
*             : pid_t DP1PID: the pid of DP1
*             : pid_t DP2PID: the pid of DP2
* RETURNS     : The error status of the proccess
*/
void processLoop(int sharedMemId, pid_t DP1PID, pid_t DP2PID)
{
    SharedMemory* pSharedMem = NULL;
    while (pSharedMem == NULL) //try to attack to shared Memory
    {
        pSharedMem = (SharedMemory*) shmat(sharedMemId, NULL, kZeroFlag); //attach to memory if valid
        if (pSharedMem == NULL)
        {
            sleep(kSleepTime);
        }
    }

    //Attached to shared memory
    //Get semaphore



    
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
        perror("Failed Shared Memory Close"); //note any errors from closing
        errorStatus = kError;
    }

    return errorStatus;
}