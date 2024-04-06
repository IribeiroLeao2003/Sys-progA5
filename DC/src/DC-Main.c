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
    //printf("Arg count: %d\n", argc);
    //printf("%s\n%s\n%s\n%s\n", argv[0], argv[1], argv[2], argv[3]);
    int errorStatus = kSuccess;
    if (argc == kArgsCount)
    {
        signal (SIGINT, shutDownHandler); //setup the sigint hanlder
        signal (SIGALRM, wakeupHandler); //setup the wakeup hanlder

        //copy args
        int sharedMemId = atoi(argv[kSharedMemLoc]);
        DP1PID = atoi(argv[kDP1ProcIDLoc]);
        DP2PID = atoi(argv[kDP2ProcIDLoc]);   
        
        //Ready to attach
        errorStatus = attachToResources(&pSharedMem, sharedMemId, &semId);
        //printf("shared mem id: %d\nsemID: %d\n", sharedMemId, semId);

        if (errorStatus != kError)
        {
            printData();
            processLoop(pSharedMem, semId);
        }

        //printf("Closing shared mem and sem\n");
        //Done process loop so shutdown
        closeSharedMem(sharedMemId);
    }


    //required printf
    printf("\nShazam !!\n");
    return errorStatus;
}



/*
* FUNCTION    : processLoop()
* DESCRIPTION : The main loop for processing the DC
* PARAMETERS  : SharedMemory* pSharedMem: the shared memory address
*             : int semID: the semaphore ID
* RETURNS     : void
*/
void processLoop(SharedMemory* pSharedMem, int semId)
{
    //printf("In proccess loop\n");
    int sleepCount = 0;
    while(shutdown == false) //until shutdown
    {
        //printf("Sleeping for 2 seconds\n");
        time_t endTime = time(NULL);
        sleep(kSleepTime);
        sleepCount += kSleepTime;
        alarm(kAlarmVal);
        if (sleepCount >= kTenSeconds) //has it been 10 seconds?
        {
            //printf("Inside PrintData\n");
            printData();
            sleepCount = 0;
        }
    }   
}



/*
* FUNCTION    : printData()
* DESCRIPTION : Prints all the data about the letter counts with symbols
* PARAMETERS  : void
* RETURNS     : void
*/
void printData()
{
    system("clear");//clear terminal
    char symbols[kSymbolsLength] = {"\0"};
    for(int i = 0; i < kLettersAtoT; i++)
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
        if (letterCount >= kStarCount)
        {
            strcat(symbols, "*");
            letterCount -= kStarCount;
        }
        else if (letterCount >= kPlusCount)
        {
            strcat(symbols, "+");
            letterCount -= kPlusCount;
        }
        else if(letterCount >= kDashCount)
        {
            strcat(symbols, "-");
            letterCount -= kDashCount;
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
    useSemaphore(semId); //need to wait for access to semaphore
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

    releaseSemaphore(semId); //release semaphore for use
    if (semctl(semId, 0, IPC_RMID) == kError) {
        perror("Failed to destroy semaphore in shutDownHandler");
    }

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
    //printf("Wakeup Handler!\n");
    useSemaphore(semId); //need to wait for access to semaphore
    //printf("Finished useSemaphore!\n");

    for (int i = 0; i <= kReadCount; i++)
    {
        int nextBufferPosition = (pSharedMem->readIndex + 1) % kBufferSize;
        if (nextBufferPosition != pSharedMem->writeIndex)
        {
            //printf("Next buffer pos: %d\n", nextBufferPosition);
            //printf("In loop index: %d\n", pSharedMem->readIndex);
            char copy = pSharedMem->buffer[pSharedMem->readIndex];
            //65 (A) - offset (65) = 0 which is the A index
            int letterCountIndex = (int) copy - kOffset;
            letterCounts[letterCountIndex]++; //increment index
            //printf("lettercount: index: %c count: %d\n", letterCountIndex + kOffset, letterCounts[letterCountIndex]);
            incrementIndex(&pSharedMem->readIndex); //increment
        }
        else
        {
            //printf("Don't wanna overtake\n");
            break;
        }
    
    }
    //printf("Finished copy!\n");

    releaseSemaphore(semId); //release semaphore for use
    //printf("Finished release!\n");

    signal (SIGALRM, wakeupHandler); //setup the sigint hanlder
}