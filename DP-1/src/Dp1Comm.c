#include "../inc/DP-1.h"




/*
* FUNCTION    : getRandomLetter()
* DESCRIPTION : Function that returns a random letter from a to t
* PARAMETERS  : None
* RETURNS     : A random char
*/
char getRandomLetter() {
    return 'a' + rand() % ('t' - 'a' + 1);
}

/*
* FUNCTION    : writeToBuffer
* DESCRIPTION : Function that returns a random letter from a to t
* PARAMETERS  : SharedMemory* shmPtr: pointer to shared memory 
              : int semId: The semaphore ID
* RETURNS     : kError if there was an error, kSuccess if the loop ended gracefully
*/
int writeToBuffer(SharedMemory* shmPtr, int semId) {
    //Define variables
    int lettWritten = 0;
    int spaceAval;
    int maxLetters = kLettersAtoT; 


    while (lettWritten < maxLetters) {
        // Get semaphore for atomic access
        if (useSemaphore(semId) == kError) {
            perror("useSemaphore error");
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

        lettWritten += maxLetters;

        //check if limit was reached, if yes sleep for 2 seconds
        if (lettWritten == kLettersAtoT) {
            sleep(kSecondsToSleep);
        }
    }
    return kSuccess;
}


void launchChildDP2(char *shmIDStr) {
    
    char pIdStr[kSharedMIDBuffer];
    //get pid
    pid_t pid = fork();


    sprintf(pIdStr, "%d", pid);

    // if PID is 0 means its a child proccess
    if (pid == 0) {  
        // turn value into string and store inside of arguments
        sprintf(pIdStr, "%d", getpid());
        char *args[] = {shmIDStr, pIdStr, NULL};  


        execv(kPathtoDP2, args);
        perror("execl");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // if its more than 0 means its a parent process
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}


