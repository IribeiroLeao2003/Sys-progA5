/*
* File: DC-Main.c
* Project: SENG2030-24W - A-05 The Hisogram System
* Programmer: Vivian Morton, Isaac Ribeiro Leao, Jakob Warywoda 
* First Version: 01-04-2024
* Description: This file contains the main logic for the DC
*/

#include "../inc/DC.h"

//required global variable for handling signals
bool shutdown = false;


//Free semaphore
//semctl (semid, 0, IPC_RMID, 0); 

int main(int argc, char* argv[])
{

    if (argc == kArgsCount)
    {
        
        signal (SIGINT, shutDownHandler); //setup the sigint hanlder
        
        int errorStatus = kSuccess;
        //copy args
        int sharedMemId = atoi(argv[kSharedMemLoc]);
        pid_t DP1PID = atoi(argv[kDP1ProcIDLoc]);
        pid_t DP2PID = atoi(argv[kDP2ProcIDLoc]);
        
        SharedMemory* pSharedMem = NULL;
        int semId = 0;
        
        //start process loop
        errorStatus = attachToResources(pSharedMem, semId, sharedMemId, &semId);

        if (errorStatus != kError)
        {

        }
        else
        {
            return kError;
        }

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
void processLoop(int sharedMemId, int semId, pid_t DP1PID, pid_t DP2PID)
{
    

    while(shutdown == false) //until shutdown
    {

    }
    killDPs(DP1PID, DP2PID);
}




void shutDownHandler(int SignalNumber)
{
    shutdown = true;
    signal (SIGINT, shutDownHandler); //setup the sigint hanlder
}