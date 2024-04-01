/*
* File: DC-Signals.c
* Project: SENG2030-24W - A-05 The Hisogram System
* Programmer: Vivian Morton, Isaac Ribeiro Leao, Jakob Warywoda 
* First Version: 01-04-2024
* Description: This file contains the signal handling logic for the DC
*/

#include "../inc/DC.h"

void killDPs(pid_t DP1PID, pid_t DP2PID)
{
    kill(DP1PID);
    kill(DP2PID);
}