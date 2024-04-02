#include "DP-1.h"



int main(){ 
    //Variables

    int smID;
    key_t smuniquekey;
    



    // getting shared memory key
    smuniquekey = ftok("../../common/bin", 'R'); 
    if (smuniquekey == kError) {
        perror("ftok");
        return kError;
    }

    //start shared memory 
    if (initSharedMem(&smID, &smuniquekey) == kSuccess) {
        printf("Shared memory started succefully.\n");
    } else {
        printf("Error in starting shared memory\n");
        return kError;
    }

    //assuming shared memory started correctly, get process ID
    pid_t dpid = fork();

    //check for the return value
    if(dpid < kSuccess){ 
        perror("fork"); 
        return kError;

    }
    else if(dpid == kSuccess){ 
        // create a string buffer for shared memory id to send to dp2
        char shmIDStr[kSharedMIDBuffer];
        sprintf(shmIDStr, "%d", smID);


        execl(kPathtoDP2exe, "dp2", shmIDStr, (char *)NULL);



    }



    return kSuccess;
}