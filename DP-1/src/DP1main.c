#include "../inc/DP-1.h"

// Global Variables
int smID;
key_t smuniquekey;
int statusBuffer;
void shutDownHandler();



int main()
{

    // create a string buffer for shared memory id to send to dp2
    char shmIDStr[kSharedMIDBuffer];
    char pIdStr[kSharedMIDBuffer];
    int errorCode;
    int semaphoreID;
    key_t semaphoreKey;

    signal (SIGINT, shutDownHandler);

    // getting shared memory key
    smuniquekey = ftok("../../common/bin", 'R');
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

    if (createSemaphore(&semaphoreID, &semaphoreKey) != kError)
    {
        printf("Semaphore Created with unique key of %d\n", semaphoreKey);

    }
    

 

    // getting semaphore ID ready
    sprintf(shmIDStr, "%d", smID);

    statusBuffer = writeToBuffer(pSharedMem, semaphoreID); 
    if(statusBuffer == kError){ 
        printf("Error as its writting to buffer  DP-1\n");
        return kError;
    }

    // launchChildDP2(shmIDStr);

   


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
