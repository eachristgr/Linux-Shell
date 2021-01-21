SHELL := /bin/bash


CC = gcc
RM = rm -f

EXECUTABLES = myShell8804

all: $(EXECUTABLES)

myShell8804: myShell8804.c
	$(CC) $< -o $@

clean:
	$(RM) *.o *~ $(EXECUTABLES)
