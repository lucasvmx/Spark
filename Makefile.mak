PARAMS=-static -static-libgcc -o wph
WARNINGS=-Wall -Wextra
SOURCES=hackpower.c
DEFINES=-D_DEBUG
CC=gcc

all: hackpower.o
	$(CC) $(SOURCES) $(PARAMS)
	
debug: hackpower.o
	$(CC) $(SOURCES) $(PARAMS) $(DEFINES)