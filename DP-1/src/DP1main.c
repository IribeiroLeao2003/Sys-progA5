#include "../inc/DP-1.h"

// Global Variables
int smID;
key_t smuniquekey;
int statusBuffer;
    int semaphoreID = 0;
void shutDownHandler();



int main()
{

    // create a string buffer for shared memory id to send to dp2
    char shmIDStr[kSharedMIDBuffer];
    char pIdStr[kSharedMIDBuffer];
    int errorCode;

    key_t semaphoreKey;

    signal (SIGINT, shutDownHandler);

    if (createSemaphore(&semaphoreID, &semaphoreKey) == kError)
    {
        //Destroy semaphore if creation failed 
        if (semctl(semaphoreID, 0, IPC_RMID) == kError) {
            perror("Failed to destroy semaphore in signal handler - DP1");
        }
        return kError;
    }
    
    
    // getting shared memory key
    smuniquekey = ftok(".", 'R');
    if (smuniquekey == kError)
    {
        perror("ftok - DP1");
        return kError;
    }

    SharedMemory *pSharedMem = NULL;

    // start shared memory
    if (initSharedMem(&smID, &smuniquekey) == kSuccess)
    {
        pSharedMem = (SharedMemory *)shmat(smID, NULL, kZeroFlag); // attach to memory if valid
    }
    else
    {
        return kError;
    }

      

   

    launchChildDP2(smID);

   

    statusBuffer = writeToBuffer(pSharedMem, semaphoreID); 
    if(statusBuffer == kError){ 
        return kError;
    }

    

   


}


/*
 * FUNCTION    : shutDownHandler()
 * DESCRIPTION : Custom handler for the SIGINT call that detaches DP-1 from memory
 * PARAMETERS  : int signalNumber: the int signal
 * RETURNS     : void
 */
void shutDownHandler()
{
    signal(SIGINT, shutDownHandler); // setup the sigint hanlder
    if (smID != kError) {
        shmctl(smID, IPC_RMID, NULL);
    }
    exit(0);
}
