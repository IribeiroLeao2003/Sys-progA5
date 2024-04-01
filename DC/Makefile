#
# File: MakeFile
# Project: SENG2030-24W- A-05 The Hisogram System
# Programmer: Vivian Morton, Isaac Ribeiro Leao, Jakob Warywoda 
# First Version: 01-04-2024
# Description: The make file for the Chat-Server side 
# =======================================================
#		Final Binary Target
# =======================================================
./bin/DC : ./obj/DC-Main.o ./obj/DC-Resources.o ../common/obj/common.o
	cc ./obj/DC-Main.o ./obj/DC-Resources.o ../common/obj/common.o -o ./bin/DC

# =======================================================
#                     File Dependencies
# =======================================================                     
./obj/DC-Main.o : ./src/DC-Main.c ./inc/DC.h ../common/inc/common.h
	cc -c -I./inc ./src/DC-Main.c -o ./obj/DC-Main.o

./obj/DC-Resources.o : ./src/DC-Resources.c ./inc/DC.h ../common/inc/common.h
	cc -c -I./inc ./src/DC-Resources.c -o ./obj/DC-Resources.o 
	
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