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
pid_t DP1PID = 0;
pid_t DP2PID = 0;


int main(int argc, char* argv[])
{
    if (argc == kArgsCount)
    {
        signal (SIGINT, shutDownHandler); //setup the sigint hanlder
        
        int errorStatus = kSuccess;
        //copy args
        int sharedMemId = atoi(argv[kSharedMemLoc]);
        DP1PID = atoi(argv[kDP1ProcIDLoc]);
        DP2PID = atoi(argv[kDP2ProcIDLoc]);
        
        SharedMemory* pSharedMem = NULL;
        int semId = 0;
        
        //Ready to attach
        errorStatus = attachToResources(pSharedMem, sharedMemId, &semId);

        if (errorStatus != kError)
        {
            processLoop(pSharedMem, semId);
        }

        //Done process loop so shutdown
        closeSharedMem(sharedMemId);
        releaseSemaphore(semId);
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
void processLoop(SharedMemory* pSharedMem, int semId)
{
    while(shutdown == false) //until shutdown
    {

    }
}




void shutDownHandler(int SignalNumber)
{
    //ensure PIDs are valid, kill and make as done
    if (DP1PID != 0)
    {
        kill(DP1PID, SIGINT);
        DP1PID = 0;
    }
    if (DP2PID != 0)
    {
        kill(DP2PID, SIGINT);
        DP2PID = 0;
    }
    shutdown = true;
    signal (SIGINT, shutDownHandler); //setup the sigint hanlder
}