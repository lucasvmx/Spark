PARAMS=-static -static-libgcc
WARNINGS=-Wall -Wextra
SOURCES=WzHack.c
DEFINES=-D_DEBUG
CC=gcc
RM=del /f /q

all: WzHack.o
	$(CC) $(SOURCES) $(PARAMS) -o WzHack

debug: WzHack.o
	$(CC) $(SOURCES) $(PARAMS) $(DEFINES) -o WzHack-Debug
	
clean:	
	$(RM) *.exe *.o

