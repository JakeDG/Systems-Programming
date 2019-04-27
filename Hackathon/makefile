# Makefile for Hackathon 
# Developed by:
# Jacob Gidley, Team Leader, jgidley@albany.edu
# Daniel Leo, Recorder, dleo2@albany.edu
# Mary Slaven, Monitor, mslaven@albany.edu

.SUFFIXES:.c .o

CC = gcc

CFLAGS = -g
OBJECTS = main.o flightFuncs.o
flights: $(OBJECTS)
	 $(CC) $(CFLAGS) $(OBJECTS) -o flights
%.o: %.c
	 $(CC) $(CFLAGS) -c $<
clean: -rm $(OBJECTS) flights
