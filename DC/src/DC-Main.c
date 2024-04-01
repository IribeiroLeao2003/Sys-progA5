/*
* File: DC-Main.c
* Project: SENG2030-24W - A-05 The Hisogram System
* Programmer: Vivian Morton, Isaac Ribeiro Leao, Jakob Warywoda 
* First Version: 01-04-2024
* Description: This file contains the main logic for the DC
*/

#include "../inc/DC.h"

//required global variable for handling signals
bool shutdown = false;
pid_t DP1PID = 0;
pid_t DP2PID = 0;
SharedMemory* pSharedMem = NULL;
int semId = 0;
int letterCounts[kLettersAtoT] = {0};


int main(int argc, char* argv[])
{
    int errorStatus = kSuccess;
    if (argc == kArgsCount)
    {
        signal (SIGINT, shutDownHandler); //setup the sigint hanlder

        //copy args
        int sharedMemId = atoi(argv[kSharedMemLoc]);
        DP1PID = atoi(argv[kDP1ProcIDLoc]);
        DP2PID = atoi(argv[kDP2ProcIDLoc]);   
        
        //Ready to attach
        errorStatus = attachToResources(pSharedMem, sharedMemId, &semId);

        if (errorStatus != kError)
        {
            processLoop(pSharedMem, semId);
        }

        //Done process loop so shutdown
        closeSharedMem(sharedMemId);
        releaseSemaphore(semId);
    }


    //required printf
    printf("Shazam !!\n");
    return errorStatus;
}



/*
* FUNCTION    : processLoop()
* DESCRIPTION : The main loop for processing the DC
* PARAMETERS  : int sharedMemID: the id of the shared memory to use
*             : pid_t DP1PID: the pid of DP1
*             : pid_t DP2PID: the pid of DP2
* RETURNS     : The error status of the proccess
*/
void processLoop(SharedMemory* pSharedMem, int semId)
{
    time_t startTime = time(NULL); //get current time
    while(shutdown == false) //until shutdown
    {
        time_t endTime = time(NULL);
        sleep(kSleepTime);

        if (startTime - endTime >= kTenSeconds) //has it been 10 seconds?
        {
            printData();
            startTime = time(NULL); //new start time
        }
    }   
}



/*
* FUNCTION    : printData()
* DESCRIPTION : Prints all the data about the letter counts
* PARAMETERS  : void
* RETURNS     : void
*/
void printData()
{
    system("clear");//clear terminal
    char symbols[kSymbolsLength] = {"\0"};
    for(int i = 0; i <= kLettersAtoT; i++)
    {
        createSymbols(symbols, letterCounts[i]);
        printf("%C-%03d %s\n", i+kOffset, letterCounts[i], symbols);
    }
}


/*
* FUNCTION    : createSymbols()
* DESCRIPTION : Creates symbol string based on the given value
* PARAMETERS  : char symbols[]: the string to write to
*             : int letterCount: the count to use
* RETURNS     : void
*/
void createSymbols(char symbols[], int letterCount)
{
    strcpy(symbols, ""); //clear string
    while (letterCount > 0)
    {
        if (letterCount >= 100)
        {
            strcat(symbols, "*");
        }
        else if (letterCount >= 10)
        {
            strcat(symbols, "+");
        }
        else if(letterCount >= 1)
        {
            strcat(symbols, "-");
        }
    }
}


/*
* FUNCTION    : shutDownHandler()
* DESCRIPTION : Custom handler for the SIGINT call that kills the DPs and switches the shutdown bool to true
* PARAMETERS  : int signalNumber: the int signal
* RETURNS     : void
*/
void shutDownHandler(int SignalNumber)
{
    //ensure PIDs are valid, kill and make as done
    if (DP1PID != 0)
    {
        kill(DP1PID, SIGINT);
        DP1PID = 0;
    }
    if (DP2PID != 0)
    {
        kill(DP2PID, SIGINT);
        DP2PID = 0;
    }
    shutdown = true;
    signal (SIGINT, shutDownHandler); //setup the sigint hanlder
}


/*
* FUNCTION    : wakeupHandler()
* DESCRIPTION : Custom handler for the SIGALRM (wake up) signal that then reads the buffer and updates the counters
* PARAMETERS  : int signalNumber: the int signal
* RETURNS     : void
*/
void wakeupHandler(int signalNumber)
{
    useSemaphore(semId); //need to wait for access to shared memory

    for (int i = 0; i <= kReadCount; i++)
    {
        char copy = pSharedMem->buffer[pSharedMem->readIndex];
            
        //65 (A) - offset (65) = 0 which is the A index
        int letterCountIndex = (int) copy - kOffset;
        letterCounts[letterCountIndex]++; //increment index

        incrementIndex(&pSharedMem->readIndex); //increment
    }


    releaseSemaphore(semId); //release semaphore for use


    signal (SIGALRM, shutDownHandler); //setup the sigint hanlder
}