# Makefile for program #2

.SUFFIXES:.c .o

CC = gcc

CFLAGS = -g
OBJECTS = main.o arcFuncs.o arcSize.o statVerify.o fileSize.o
prog:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o archiver
%.o: %.c
	$(CC) $(CFLAGS) -c $<


clean:rm -f *.o core
