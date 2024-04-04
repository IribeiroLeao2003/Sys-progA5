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
            printf ("Releasing Semaphore  DP-1\n");
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
                printf ("Read Index Reached ! DP-1\n");
                break; 
            }
        }
         printf ("Ammount of letters written %d/Releasing semaphore DP-1\n", lettWritten);
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


