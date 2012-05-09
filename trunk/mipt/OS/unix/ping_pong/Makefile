#MAKEFILE

CC              = gcc
CFLAGS          = -std=c99 -Wall
LFLAGS          = -L.
SYS_LIBS        = -lstring -lpthread -lnamed_sem

TARGETS = processes_impl.o
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:%.c=%.o)

#---------- targets --------------------------------------
all:  $(TARGETS)

%.o:	%.c
			$(CC) $(CFLAGS) $(LFLAGS) $(SYS_LIBS) $*.c -o $@

#---------- target 1 -------------------------------------
#  C  target
ping_pong:  processes_impl.c
						$(CC) $(LFLAGS) -o $@ $@.o $(SYS_LIBS)

#---------- clear up -------------------------------------
clean:
				rm  --force  $(EXECUTABLE) $(OBJECTS) $(PREREQUISITES)
 
