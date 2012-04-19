#MAKEFILE

CC              = gcc
CFLAGS          = -std=c99 -Wall
LFLAGS          = 
SYS_LIBS        = 

TARGETS	= backup.o

#---------- targets --------------------------------------
all:	$(TARGETS)

%.o:	%.c
			$(CC) $(CFLAGS) $(SYS_LIBS) $*.c -o $@

#---------- target 1 -------------------------------------
#  C  target
backup:	backup.c
							$(CC) $(LFLAGS) -o $@ $@.o  $(SYS_LIBS)

#---------- clear up -------------------------------------
clean:
				rm  --force  $(EXECUTABLE) $(OBJECTS) $(PREREQUISITES)
 
