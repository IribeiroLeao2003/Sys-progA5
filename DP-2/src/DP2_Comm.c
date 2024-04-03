#include "DP2.h"

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
}