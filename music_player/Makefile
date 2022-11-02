GBDK_HOME = ../../gbdk/

LCC = $(GBDK_HOME)bin/lcc

PROJECTNAME = player
BINS	    = $(PROJECTNAME).gb
CSOURCES   := $(wildcard src/*.c) $(wildcard src/songs/*.c)

all:	$(BINS)

# Compile and link all source files in a single call to LCC
$(BINS):	$(CSOURCES)
	$(LCC) -Isrc -o $@ hUGEDriver.obj.o $(CSOURCES)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm
