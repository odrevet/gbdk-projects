GBDK_HOME = ../../gbdk/

LCC = $(GBDK_HOME)bin/lcc 

PROJECTNAME = linda
BINS	    = $(PROJECTNAME).gb
CSOURCES   := $(wildcard src/*.c) $(wildcard src/graphics/*.c) $(wildcard src/maps/*.c) $(wildcard src/songs/*.c)

all:	$(BINS)

$(BINS):	$(CSOURCES)
	$(LCC) -Isrc -Wa-l -Wl-m -Wl-j -o $@ hUGEDriver.obj.o $(CSOURCES)

clean:
	rm -f *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm

