#include "DP-1.h" 

char getRandomLetter() {
    return 'a' + rand() % ('t' - 'a' + 1);
}


void writeToBuffer(SharedMemory* shmPtr, int semId) {
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
            shmPtr->buffer[shmPtr->writeIndex] = generate_random_letter();
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
}