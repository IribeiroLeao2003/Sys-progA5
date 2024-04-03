#include "../inc/DP-1.h"

// Global Variables
int smID;
key_t smuniquekey;
int statusBuffer;

int main()
{

    // create a string buffer for shared memory id to send to dp2
    char shmIDStr[kSharedMIDBuffer];
    char pIdStr[kSharedMIDBuffer];
    int errorCode;
    int semaphorePtr;
    // will set up later
    // signal (SIGINT, shutDownHandler);

    // getting shared memory key
    smuniquekey = ftok("../../common/bin", 'S');
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

    // assuming shared memory started correctly, get process ID
    pid_t dpid = getpid();

    // check for the return value
    if (dpid < kSuccess)
    {
        perror("fork");
        return kError;
    }
    else
    {

        // getting semaphore ID ready
        sprintf(shmIDStr, "%d", smID);
        sprintf(pIdStr, "%d", dpid);

        // execl(kPathtoDP2, "dp2", shmIDStr, (char *)NULL);

        // For DC
        char *args[3];
        args[0] = shmIDStr;
        args[1] = pIdStr;
        args[2] = k0String;

        execv(kPathtoDC, args); // start with args
    }

    statusBuffer = writeToBuffer(pSharedMem, kSemaphoreID);
    if (statusBuffer == kError)
    {
        printf("Error on writting to buffer");
        return kError;
    }

    return kSuccess;
}

/*
 * FUNCTION    : shutDownHandler()
 * DESCRIPTION : Custom handler for the SIGINT call that detaches DP-1 from memory
 * PARAMETERS  : int signalNumber: the int signal
 * RETURNS     : void
 */
void shutDownHandler()
{

    if (smuniquekey != kError)
    {
    }
    signal(SIGINT, shutDownHandler); // setup the sigint hanlder
}
