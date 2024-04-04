#
# File: MakeFile
# Project: SENG2030-24W- A-05 The Hisogram System
# Programmer: Vivian Morton, Isaac Ribeiro Leao, Jake Warywoda 
# First Version: 01-04-2024
# Description: The make file for Data Producor 2
# =======================================================
#		Final Binary Target
# =======================================================
./bin/DP2 : ./obj/DP2_Main.o ./obj/DP2_Comm.o ../common/obj/common.o
	cc ./obj/DP2_Main.o ./obj/DP2_Comm.o ../common/obj/common.o -o ./bin/DP2

# =======================================================
#                     File Dependencies
# =======================================================                     
./obj/DP2_Main.o : ./src/DP2_Main.c ./inc/DP2.h ../common/inc/common.h
	cc -c -I./inc ./src/DP2_Main.c -o ./obj/DP2_Main.o

./obj/DP2_Comm.o : ./src/DP2_Comm.c ./inc/DP2.h ../common/inc/common.h
	cc -c -I./inc ./src/DP2_Comm.c -o ./obj/DP2_Comm.o 
	
../common/obj/common.o : ../common/src/common.c ../common/inc/common.h
	cc -c -I/inc ../common/src/common.c -o ../common/obj/common.o
	
#
# =======================================================
# Other targets
# =======================================================                     
all : ./bin/DP2

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	rm -f ../common/obj/*.o
	rm -f ../common/bin/*