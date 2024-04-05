/*
 *  File            :   DP2_Main.c
 *  Project         :   SENG2030-24W - A-05 The Histogram System
 *  Programmer      :   Jake Warywoda, Vivian Morton, Isaac Ribeiro Leao
 *  First Version   :   02-04-2024
 *  Description     :   This file contains the main logic for the second
 *                      data producer process - DP2
*/

#include "DP2.h"

// Globals
int shmID;
key_t shmKey;
pid_t DP1_pid;
pid_t DP2_pid;
SharedMemory* pSharedMemory = NULL;
int semaphoreID = 0;
key_t semaphoreKey;

volatile sig_atomic_t running = 1;


int main(int argc, char* argv[]) {
    int semaphoreResult;
    int writeCounter = 0;

    // Check the arg
    if (argc != 2) {
        fprintf(stderr, "shared memory ID error");
        exit(EXIT_FAILURE);
    }

    // Store shmID passed from DP1
    shmID = atoi(argv[1]);
 

    // Register SIGINT handler
    signal(SIGINT, sigintHandler);

    // Get PID of itself 
    DP2_pid = getpid();

    // Get PID of DP1 (parent)
    DP1_pid = getppid();

    // Spawn DC
    spawnDC(shmID, DP1_pid);



    // Attach to shared memory
    pSharedMemory = (SharedMemory*)shmat(shmID, NULL, kZeroFlag);
    if (pSharedMemory == (void*)kError) {
        // Attaching failure!
        perror("Error attaching to shared memory");
        // Exit app with failure
        exit(EXIT_FAILURE);
    }
    else{ 
        printf("Shared memory attached succesfully!\n");
    }

    // Get the semaphore information
    semaphoreResult = getSemaphoreInfo(&semaphoreID, &semaphoreKey);
    if (semaphoreResult == kError) {
        perror("Semaphore failure - DP2");
        exit(EXIT_FAILURE);
    }

    // Enter the main loop of writing to circular buffer in shared memory
    while(running) {
        int writeResult = writeLetterToBuffer(pSharedMemory, semaphoreID);

        if (writeResult == kSuccess) {
            writeCounter++;
        }
        
        // Sleep for 1/20th second (50,000 microseconds)
        // usleep(kOneTwentieth);
    }

    printf("DP2 wrote %d times\n", writeCounter);
    return kSuccess;
}


/*
 *  FUNCTION    :   sigintHandler()
 *  DESCRIPTION :   This function is the registered signal handler
 *                  for the SIGINT signal
 *  PARAMETERS  :   int -   sig: the signal
 *  RETURNS     :   void
*/
void sigintHandler(int sig) {
    // Flag exiting the loop
    running = 0;

    // Detach from shared memory
    if (pSharedMemory != NULL) {
        if (shmdt(pSharedMemory) == kError) {
            perror("shmdt error - DP2");
        }
        pSharedMemory = NULL;
    }
}