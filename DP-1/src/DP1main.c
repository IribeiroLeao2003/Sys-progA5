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

    if (createSemaphore(&semaphoreID, &semaphoreKey) != kError)
    {
        printf("Semaphore Created with unique ID of  %d\n", semaphoreID);

    }
    else {
        printf("Semaphore create failed\n");
        return kError;
    }
    
    
    // getting shared memory key
    smuniquekey = ftok(".", 'R');
    if (smuniquekey == kError)
    {
        perror("ftok");
        return kError;
    }

    SharedMemory *pSharedMem = NULL;

    // start shared memory
    if (initSharedMem(&smID, &smuniquekey) == kSuccess)
    {
        printf("Shared memory started succefully with key of %d\n", smuniquekey);
        pSharedMem = (SharedMemory *)shmat(smID, NULL, kZeroFlag); // attach to memory if valid
    }
    else
    {
        printf("Error in starting shared memory\n");
        return kError;
    }

      // getting semaphore ID ready

    printf("Sending %d DP1\n", smID);

    launchChildDP2(smID);

 

    statusBuffer = writeToBuffer(pSharedMem, semaphoreID); 
    if(statusBuffer == kError){ 
        printf("Error as its writting to buffer  DP-1\n");
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
    if (semctl(semaphoreID, 0, IPC_RMID) == -1) {
        perror("Failed to destroy semaphore in signal handler");
    }
    exit(0);

}
