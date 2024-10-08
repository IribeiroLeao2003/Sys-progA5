/*
 *  File            :   DP2_Comm.c
 *  Project         :   SENG2030-24W - A-05 The Histogram System
 *  Programmer      :   Jake Warywoda, Vivian Morton, Isaac Ribeiro Leao
 *  First Version   :   02-04-2024
 *  Description     :   This file contains the functions pertinent to the second
 *                      data producing process (DP2)
*/


#include "DP2.h"


/*
 * FUNCTION     :   spawnDC() 
 * DESCRIPTION  :   This function is used to spawn the DC process,
 *                  passing it the shared memory ID, and the PID of 
 *                  DP1 and DP2
 * PARAMETERS   :   int     -   shmID: the shared memory ID
 *                  pid_t   -   DP1_pid: the PID of DP1
 * RETURNS      :   void
*/
void spawnDC(int shmID, pid_t DP1_pid) {
    // Char buffers for passing when spawning DC
    char shmID_string[kshmIDBufferSize];
    char DP1_p[kPIDSize];
    char DP2_p[kPIDSize];

    // Get PID of running DP2 process
    pid_t DP2_Current = fork();

    // If DP2_Current is 0, child process which can spawn DC
    if (DP2_Current == 0) {
        // Get parent process PID
        pid_t DP2_Parent = getppid();

        // Fill buffers
        sprintf(shmID_string, "%d", shmID);
        sprintf(DP1_p, "%d", DP1_pid);
        sprintf(DP2_p, "%d", DP2_Parent);

        execl(kPathToDCexe, kDCProcessName, shmID_string, DP1_p, DP2_p, NULL);
        // If execl returns, error occurec
        perror("execl DC spawn error");
        exit(EXIT_FAILURE);
    } else if (DP2_Current > 0) {
        // Parent process
    } else {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

}



/*
 * FUNCTION     :   writeLetterToBuffer() 
 * DESCRIPTION  :   This function is used to write a single random letter 
 *                  from 'a' to 't' to the circular buffer, shared with 
 *                  data producer 1 (DP1) and data consumer (DC). Utilises
 *                  a semaphore to marshal shared memory access              
 * PARAMETERS   :   SharedMemory*   -   pSharedMemory: pointer to shared memory
 *                  int             -   semaphoreID: the ID of the semaphore
 * RETURNS      :   int -   1: EXIT_FAILURE
 *                      -   0: kSuccess (successful write and sleep)
*/
int writeLetterToBuffer(SharedMemory* pSharedMemory, int semaphoreID) {
    // First lets lock the semaphore
   // Lock the semaphore for atomic access
    if (semop(semaphoreID, &getSem, 1) == kError) {
        perror("semop lock error - DP2");
        exit(EXIT_FAILURE);
    }

    // Calculate the next position to write to, ensuring circular buffer 
    int nextBufferPosition = (pSharedMemory->writeIndex + 1) % kBufferSize;

    if (nextBufferPosition != pSharedMemory->readIndex) {
  
        // Generate a random letter and write it to the buffer
        char letter = getRandomLetter();
        pSharedMemory->buffer[pSharedMemory->writeIndex] = letter;
        //printf("DP2 writes '%c' at position %d\n", letter, pSharedMemory->writeIndex);

        // Update the write index to the next position
        pSharedMemory->writeIndex = nextBufferPosition;

        // Sleep for 1/20th of a second to fulfill the requirement on the documentation
        usleep(kOneTwentieth); // 50,000 microseconds = 1/20th of a second
    } else {
        // printf("DP2 doesn't want to overtake readIndex\n");
        usleep(kOneTwentieth);
        return kDontWrite;
    }

    // Release the semaphore
    if (semop(semaphoreID, &releaseSem, 1) == -1) {
        perror("semop unlock error - DP2");
        exit(EXIT_FAILURE);
    }

    return kSuccess;
}



/*
 * FUNCTION     :   getSemaphoreInfo() 
 * DESCRIPTION  :   This function is used to acquire the semaphore key
 *                  and then the semaphore ID
 * PARAMETERS   :   int*    -   semaphoreID: pointer to global semaphoreID
 *                  key_t*  -   semaphoreKey: pointer to global semaphoreKey
 * RETURNS      :   int -   -1: kError - error arose
 *                      -   0: kSuccess
*/
int getSemaphoreInfo(int* semaphoreID, key_t* semaphoreKey) {
    // Generate the semaphore key using ftok
    *semaphoreKey = ftok(kPathCommonBin, kSemaphoreID); // ../../common/bin
    if (*semaphoreKey == (key_t)kError) {
        perror("ftok failure - semaphore error DP2");
        return kError;
    }

    // printf("DP2 semKey is : %d\n", *semaphoreKey);
    // Get the ID
    *semaphoreID = semget(*semaphoreKey, kSingleUseSemaphore, kZeroFlag);
    if (*semaphoreID == kError) {
        perror("semget error - DP2");
        return kError;
    }
    // printf("DP2 semID is : %d\n", *semaphoreID);

    return kSuccess;
}