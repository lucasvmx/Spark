PARAMS=-static -static-libgcc -o wph
WARNINGS=-Wall -Wextra
SOURCES=hack.c
DEFINES=-D_DEBUG
CC=gcc

all: hack.o
	$(CC) $(SOURCES) $(PARAMS)
	
debug: hack.o
	$(CC) $(SOURCES) $(PARAMS) $(DEFINES)