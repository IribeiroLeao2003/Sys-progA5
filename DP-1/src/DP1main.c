#include "../inc/DP-1.h"






//Global Variables
int smID;
key_t smuniquekey;
int statusBuffer; 


int main(){ 
    

    
    // will set up later
    //signal (SIGINT, shutDownHandler);

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
    
    statusBuffer = writeToBuffer(smuniquekey, kSemaphoreID );
    if(statusBuffer == kError){ 
        printf("Error on writting to buffer"); 
        return kError;
    }


    return kSuccess;
}


/*
* FUNCTION    : shutDownHandler()
* DESCRIPTION : Custom handler for the SIGINT call that detaches DP-1 from memory
* PARAMETERS  : int signalNumber: the int signal
* RETURNS     : void
*/
void shutDownHandler()
{

    if(smuniquekey != kError){ 

    }
    signal (SIGINT, shutDownHandler); //setup the sigint hanlder
}
