GBDK_HOME = ../../gbdk/

LCC = $(GBDK_HOME)bin/lcc
# LCC = $(LCC) -debug

PROJECTNAME = marioland
BINS	    = $(PROJECTNAME).gb
CSOURCES   := $(wildcard src/*.c) $(wildcard src/graphics/*.c) $(wildcard src/maps/*.c) $(wildcard src/songs/*.c)

all:	$(BINS)

# Compile and link all source files in a single call to LCC
$(BINS):	$(CSOURCES) $(ASMSOURCES)
	$(LCC)  -o $@ hUGEDriver.o $(CSOURCES) $(ASMSOURCES)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm
