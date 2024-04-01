#include "../inc/DC.h"




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



void processLoop(int sharedMemId, pid_t DP1PID, pid_t DP2PID)
{
    SharedMemory* pSharedMem = NULL;
    while (pSharedMem == NULL)
    {
        pSharedMem = (SharedMemory*) shmat(sharedMemId, NULL, kZeroFlag); //attach to memory if valid
        sleep(kSleepTime);
    }
    
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