# Makefile for project 5 
# Developed by:
# Jacob Gidley, Team Leader, jgidley@albany.edu
# Daniel Leo, Recorder, dleo2@albany.edu
# Mary Slaven, Monitor, mslaven@albany.edu

.SUFFIXES:.c .o

CC = gcc
CFLAGS = -g
PROGRAMS = shell list create
EXECUTABLES = simpleshell list create

SHELLOBJECTS = simpleshell.o shellFunctions.o input.o
LISTOBJECTS = list.o
CREATEOBJECTS = create.o

makefile: $(PROGRAMS)

shell: $(SHELLOBJECTS)
	 $(CC) $(CFLAGS) $(SHELLOBJECTS) -o simpleshell
%.o: %.c
	 $(CC) $(CFLAGS) -c $<
	 
list: $(LISTOBJECTS)
	 $(CC) $(CFLAGS) $(LISTOBJECTS) -o list
%.o: %.c
	 $(CC) $(CFLAGS) -c $<
	 
create: $(CREATEOBJECTS)
	 $(CC) $(CFLAGS) $(CREATEOBJECTS) -o create
%.o: %.c
	 $(CC) $(CFLAGS) -c $<
	 
clean: 
	rm -f *.o $(EXECUTABLES)
