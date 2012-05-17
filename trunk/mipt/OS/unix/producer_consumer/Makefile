#MAKEFILE

CC              = gcc
CFLAGS          = -std=c99 -Wall
LFLAGS          = -L.
SYS_LIBS        = -lstring -lpthread

TARGETS = threads_impl.o
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:%.c=%.o)

#---------- targets --------------------------------------
all:  $(TARGETS)

%.o:	%.c
			$(CC) $(CFLAGS) $(LFLAGS) $*.c -o $@ $(SYS_LIBS)

#---------- target 1 -------------------------------------
#  C  target
producer_consumer:  threads_impl.c
						$(CC) $(LFLAGS) -o $@ $@.o $(SYS_LIBS)

#---------- clear up -------------------------------------
clean:
				rm  --force  $(EXECUTABLE) $(OBJECTS) $(PREREQUISITES)
 
