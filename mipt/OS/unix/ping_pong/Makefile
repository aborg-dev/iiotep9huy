#MAKEFILE

CC              = gcc
CFLAGS          = -std=c99 -Wall
LFLAGS          = 
SYS_LIBS        = -lpthread

TARGETS = ping_pong.o
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:%.c=%.o)

#---------- targets --------------------------------------
all:  $(TARGETS)

%.o:	%.c
			$(CC) $(CFLAGS) $(SYS_LIBS) $*.c -o $@

#---------- target 1 -------------------------------------
#  C  target
ping_pong:  ping_pong.c
						$(CC) $(LFLAGS) -o $@ $@.o  $(SYS_LIBS)

#---------- clear up -------------------------------------
clean:
				rm  --force  $(EXECUTABLE) $(OBJECTS) $(PREREQUISITES)
 
