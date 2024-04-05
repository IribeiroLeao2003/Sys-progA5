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
        else { 
            printf ("Entering critical region  DP-1\n");
        }

        // Calculate space available to write and change lettersToWrite if necessary
        if (shmPtr->writeIndex >= shmPtr->readIndex) {
            spaceAval = kBufferTop - shmPtr->writeIndex + shmPtr->readIndex;
        } else {
            spaceAval = shmPtr->readIndex - shmPtr->writeIndex;
        }

        if (spaceAval <= 0) {
            //if there is no more space simply release the semaphore
            printf ("Not Enought Space !, Releasing Semaphore  DP-1\n");
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
            printf("DP2 writes '%c' at position %d\n", shmPtr->buffer[shmPtr->writeIndex] , shmPtr->writeIndex);

            incrementIndex(&(shmPtr->writeIndex));
                //check if we reached the read intex
            if (nextIndex == shmPtr->readIndex) {
                printf ("Read Index Reached ! DP-1\n");
                break; 
            }
     
        }

        
         printf ("Releasing semaphore DP-1\n");
        //After finishing writting, release semaphore using semID
        if (releaseSemaphore(semId) == kError) {
            perror("releaseSemaphore");
            return kError;
        }
                // After releasing the semaphore (indicating end of critical section)
        int usedSpace = (shmPtr->writeIndex >= shmPtr->readIndex) ?
                        (shmPtr->writeIndex - shmPtr->readIndex) :
                        (kBufferSize - shmPtr->readIndex + shmPtr->writeIndex);
        printf("Buffer usage: %d/%d\n", usedSpace, kBufferSize);

     


        sleep(kSecondsToSleep);
    
    }
    return kSuccess;
}


void launchChildDP2(int smID) {
    char args[30];
    //get pid
    pid_t pid = fork();

    switch (pid)
    {
    case kError:
        perror("fork");
        exit(EXIT_FAILURE);
        break;

    case 0 : 
        printf("Im the child, and my PID is %d - DP1\n", pid);
        sprintf(args, "%d", smID);
        printf("Sending %s DP1\n", args);
        execl(kPathtoDP2, "DP-2",  args, NULL);
        perror("execl");
        exit(EXIT_FAILURE);
        break;
    
    default:
        printf("Im the parent, and my PID is %d\n", pid);
        // if its more than 0 means its a parent process
        break;
    }

}


