PARAMS=-static -static-libgcc

SHARED_PARAMS=-static -static-libgcc -shared -g0
SHARED_OUTPUT=libwzhack.a
SHARED_DEFINES=-D_SHARED_LIB

WARNINGS=-Wall -Wextra
SOURCES=src/wzhack.cpp
DEFINES=-D_DEBUG

DLL_DEFINES=-D_WINDLL -D_EXPORT_FUNCTIONS
DLL_OUTPUT=WzHack.dll
DLL_PARAMS=-mdll -g0

CPP=g++
RM=del /f /q

.PHONY: debug
.PHONY: dll
.PHONY: shared

all:
	$(CPP) $(SOURCES) $(PARAMS) -o WzHack.exe
	
dll:
	$(CPP) $(SOURCES) $(PARAMS) $(DLL_PARAMS) $(DLL_DEFINES) -o $(DLL_OUTPUT)
	
shared:
	$(CPP) $(SOURCES) $(SHARED_PARAMS) $(SHARED_DEFINES) -o $(SHARED_OUTPUT)
	
debug:
	$(CPP) $(SOURCES) $(PARAMS) $(DEFINES) -o WzHack-Debug.exe
	
clean:	
	$(RM) *.exe *.o *.dll

