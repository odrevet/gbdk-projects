GBDK_HOME = ../gbdk/

LCC = $(GBDK_HOME)bin/lcc 
# LCC = $(LCC) -debug

PROJECTNAME = GAME
BINS	    = $(PROJECTNAME).gb
CSOURCES   := $(wildcard *.c) $(wildcard graphics/*.c) $(wildcard maps/*.c)
ASMSOURCES := $(wildcard *.s)

all:	$(BINS)

# Compile and link all source files in a single call to LCC
$(BINS):	$(CSOURCES) $(ASMSOURCES)
	$(LCC) -o $@ $(CSOURCES) $(ASMSOURCES)

run: 
	sameboy $(BINS)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm

