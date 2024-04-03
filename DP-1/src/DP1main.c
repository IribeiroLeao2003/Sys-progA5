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
    int semaphorePtr;

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
        printf("Shared memory started succefully.\n");
        pSharedMem = (SharedMemory *)shmat(smID, NULL, kZeroFlag); // attach to memory if valid
    }
    else
    {
        printf("Error in starting shared memory\n");
        return kError;
    }

    if (createSemaphore(&semaphorePtr, &smuniquekey) == kError)
    {
        printf("Error creating semaphore semaphore");
        return kError;
    }

 

    // getting semaphore ID ready
    sprintf(shmIDStr, "%d", smID);

    launchChildDP2(shmIDStr);

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
