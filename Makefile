#
# Makefile 
#
# Type  make    to compile all the programs
# in the chapter 
#

HEADER  = Header.h
OBJS 	= remoteClient.o ReadWrite.o
SOURCE	= remoteClient.c ReadWrite.c
OUT  	= remoteClient
SOBJS 	= dataServer.o
SOBJS1 	= ReadWrite.o DataFunctions.o Functions.o QueueData.o QueueFunctions.o
SSOURCE	= dataServer.c
SSOURCE1	= ReadWrite.c DataFunctions.c Functions.c QueueData.c QueueFunctions.c
SOUT  	= dataServer
CC	= gcc

all:clean dataServer remoteClient

remoteClient:  remoteClient.o ReadWrite.o
	$(CC) -o ./ClientDir/$(OUT) $(OBJS)

dataServer:  dataServer.o ReadWrite.o DataFunctions.o Functions.o QueueData.o QueueFunctions.o
	$(CC) -o ./ServerDir/$(SOUT) $(SOBJS) -lpthread $(SOBJS1)

#seperate compilation
remoteClient.o: remoteClient.c
	$(CC) -c remoteClient.c
   
dataServer.o: dataServer.c -lpthread
	$(CC) -c dataServer.c
   
ReadWrite.o: ReadWrite.c
	$(CC) -c ReadWrite.c
   
DataFunctions.o: DataFunctions.c
	$(CC) -c DataFunctions.c
   
Functions.o: Functions.c
	$(CC) -c Functions.c

QueueData.o: QueueData.c
	$(CC) -c QueueData.c
   
QueueFunctions.o: QueueFunctions.c
	$(CC) -c QueueFunctions.c

#clean
clean:
	clear
	rm -f ./ClientDir/$(OUT) $(OBJS)
	rm -f ./ServerDir/$(SOUT) $(SOBJS) $(SOBJS1)

