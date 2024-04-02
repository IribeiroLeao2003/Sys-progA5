#
# File: MakeFile
# Project: SENG2030-24W- A-05 The Hisogram System
# Programmer: Vivian Morton, Isaac Ribeiro Leao, Jakob Warywoda 
# First Version: 01-04-2024
# Description: The make file for the Chat-Server side 
# =======================================================
#		Final Binary Target
# =======================================================
./bin/DP1 : ./obj/DP1main.o ./obj/Dp1Comm.o ../common/obj/common.o
	cc ./obj/DP1main.o ./obj/Dp1Comm.o ../common/obj/common.o -o ./bin/DP1

# =======================================================
#                     File Dependencies
# =======================================================                     
./obj/DP1main.o : ./src/DP1main.c ./inc/DP-1.h ../common/inc/common.h
	cc -c -I./inc ./src/DP1main.c -o ./obj/DP1main.o

./obj/Dp1Comm.o : ./src/Dp1Comm.c ./inc/DP-1.h ../common/inc/common.h
	cc -c -I./inc ./src/Dp1Comm.c -o ./obj/Dp1Comm.o 
	
../common/obj/common.o : ../common/src/common.c ../common/inc/common.h
	cc -c -I/inc ../common/src/common.c -o ../common/obj/common.o
	
#
# =======================================================
# Other targets
# =======================================================                     
all : ./bin/chat-server

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	rm -f ../common/obj/*.o
	rm -f ../common/bin/*