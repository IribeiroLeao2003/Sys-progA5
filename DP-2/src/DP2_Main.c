#include "DP2.h"

/*
DP2 is launched via an exec() call 
    DP2 takes the sharedMemory ID (shmID) as a command line argument
Once launched:
    Get the processID (PID) of ITSELF and the processID of DP1 (parent)
    DP2 LAUNCHES the DC (via exec() call) and passes 3 command line args:
        DP2 PID
        DP1 (parent) PID
        shmID 
THEN DP2 attaches to the shared memory (not before spawning DC)
Now DP2 enters main loop (DP1 also enters main loop)
DP1/DP2 both will write to circular buffer:
    Guard writing using semaphore
    DP2 generates one random letter and writes it to buffer, sleeping for 1/20 second
        atomic operation
Both DP1/DP2 are accessing the same write index to write to circular buffer 
in shared memory (hence semaphore)
    Once DP1/DP2 writes 256th element of buffer -> write index wraps back to
    position 0 in order for DP processes to continue writing
DP1/DP2 must ensure they never write past the circular buffers read index
(which is used by the DC)
    If either DP process detects its reached read index, it will sleep and
    then check again to see if DC has caught up
DP1/DP2 must both listen for and ahndle SIGINT signal 
    Register signal handler
    When caught, each DP process just detaches from shared memory and exits
        No statements
*/