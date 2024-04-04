#include "DP2.h"

/*
void spawnDC(int* shmID, pid_t DP1_pid, pid_t DP2_pid) {
    // Char buffers for passing when spawning DC
    char shmID_string[kshmIDBufferSize];
    char DP1_p[kPIDSize];
    char DP2_p[kPIDSize];

    // Fill the buffers
    sprintf(shmID_string, "%d", shmID);
    sprintf(DP1_p, "%d", DP1_pid);
    sprintf(DP2_p, "%d", DP2_pid);

    // Use execl with our 3 args to pass
    execl(kPathToDCexe, kDCProcessName, shmID_string, DP1_p, DP2_p, (char *)NULL);
} */

void spawnDC(int* shmID, pid_t DP1_pid) {
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
* FUNCTION    : getRandomLetter()
* DESCRIPTION : Function that returns a random letter from a to t
* PARAMETERS  : None
* RETURNS     : A random char
*/
char getRandomLetter() {
    return 'a' + rand() % ('t' - 'a' + 1);
}


int writeLetterToBuffer(SharedMemory* pSharedMemory, int semID) {

}


int getSemaphoreInfo(int* semaphoreID, key_t* semaphoreKey) {
    // Generate the semaphore key using ftok
    *semaphoreKey = ftok(kPathCommonBin, kSemaphoreID); // ../../common/bin
    if (*semaphoreKey == (key_t)kError) {
        perror("ftok failure - semaphore error DP2");
        return kError;
    }

    // Get the ID
    *semaphoreID = semget(*semaphoreKey, kSingleUseSemaphore, kZeroFlag);
    if (*semaphoreID == kError) {
        perror("semget error - DP2");
        return kError;
    }

    return kSuccess;
}