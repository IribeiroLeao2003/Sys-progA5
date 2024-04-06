#include "../inc/DP-1.h"

/*
* FUNCTION    : writeToBuffer
* DESCRIPTION : Function that returns a random letter from a to t
* PARAMETERS  : SharedMemory* shmPtr: pointer to shared memory 
              : int semId: The semaphore ID
* RETURNS     : kError if there was an error, kSuccess if the loop ended gracefully
*/
int writeToBuffer(SharedMemory* shmPtr, int semId) {
    //Define variables
    int spaceAval;
    int maxLetters = kLettersAtoT; 


  

    while (true) {
        // Get semaphore for atomic access
        if (useSemaphore(semId) == kError) {
            fprintf(stderr, "useSemaphore failed: %s\n", strerror(errno));
            return kError;
        }

        // Calculate space available to write and change lettersToWrite if necessary
        if (shmPtr->writeIndex >= shmPtr->readIndex) {
            spaceAval = kBufferTop - shmPtr->writeIndex + shmPtr->readIndex;
        } else {
            spaceAval = shmPtr->readIndex - shmPtr->writeIndex;
        }

        if (spaceAval <= 0) {
            //if there is no more space simply release the semaphore
            releaseSemaphore(semId);

            //sleep for one milesecond
            usleep(kOneMileSecond); 
            continue;
        }

        if (spaceAval >= kLettersAtoT) {
            maxLetters = kLettersAtoT;
        } else {
            maxLetters = spaceAval;
        }

        // write the letters to buffer
        for (int i = 0; i < maxLetters; i++) {
            int nextIndex = shmPtr->writeIndex;
            shmPtr->buffer[shmPtr->writeIndex] = getRandomLetter();

            incrementIndex(&(shmPtr->writeIndex));
                //check if we reached the read intex
            if (nextIndex == shmPtr->readIndex) {
                break; 
            }
     
        }

        
        //After finishing writting, release semaphore using semID
        if (releaseSemaphore(semId) == kError) {
            perror("releaseSemaphore");
            return kError;
        }
     


        sleep(kSecondsToSleep);
    
    }
    return kSuccess;
}






/*
* FUNCTION    : launchChildDP2
* DESCRIPTION : Function that creates child proccess and sets it to be DP-2
* PARAMETERS  : int smID: The Shared Memory ID that will be sent to DP-2
* RETURNS     : Nothing
*/
void launchChildDP2(int smID) {
    char args[kArgsSize];
    //get pid
    pid_t pid = fork();

    switch (pid)
    {
    case kError:
        perror("fork");
        exit(EXIT_FAILURE);
        break;

    case kChildProccess: 
        sprintf(args, "%d", smID);
        execl(kPathtoDP2, "DP-2",  args, NULL);
        perror("execl");
        exit(EXIT_FAILURE);
        break;
    
    default:
        break;
    }

}


