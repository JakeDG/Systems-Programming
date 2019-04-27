# Makefile for program #3 
# Developed by:
# Jacob Gidley, Team Leader, jgidley@albany.edu
# Daniel Leo, Recorder, dleo2@albany.edu
# Mary Slaven, Monitor, mslaven@albany.edu

.SUFFIXES:.c .o

CC = gcc

CFLAGS = -g
OBJECTS = main.o browser.o miscFuncs.o index.o listfuncs.o
indexer: $(OBJECTS)
	 $(CC) $(CFLAGS) $(OBJECTS) -o indexer
%.o: %.c
	 $(CC) $(CFLAGS) -c $<
clean: 
	-rm $(OBJECTS) indexer
