PARAMS=-static -static-libgcc
WARNINGS=-Wall -Wextra
SOURCES=WzHack.c
DEFINES=-D_DEBUG
DLL_DEFINES=-D_WINDLL
DLL_OUTPUT=WzHack.dll
DLL_PARAMS=-mdll
CC=gcc
RM=del /f /q

all: WzHack.o
	$(CC) $(SOURCES) $(PARAMS) -o WzHack
	
dll: WzHack.o
	$(CC) $(SOURCES) $(PARAMS) $(DLL_PARAMS) $(DLL_DEFINES) -o $(DLL_OUTPUT)
	
debug: WzHack.o
	$(CC) $(SOURCES) $(PARAMS) $(DEFINES) -o WzHack-Debug
	
clean:	
	$(RM) *.exe *.o *.dll

