# Makefile for Project 4
# Developed by:
# Jacob Gidley, Recorder, jgidley@albany.edu
# Daniel Leo, Team Leader, dleo2@albany.edu
# Mary Slaven, Monitor, mslaven@albany.edu

.SUFFIXES:.c .o

CC = gcc

CFLAGS = -g
OBJECTS = main.o bstFuncs.o hashTableFuncs.o hashFunction.o
firstpass: $(OBJECTS)
	 $(CC) $(CFLAGS) $(OBJECTS) -o firstpass
%.o: %.c
	 $(CC) $(CFLAGS) -c $<
clean: -rm $(OBJECTS) firstpass
