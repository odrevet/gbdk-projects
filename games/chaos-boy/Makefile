LCC = $(GBDK_HOME)/bin/lcc
PROJECTNAME = GAME
BINS	    = $(PROJECTNAME).gb
CSOURCES   := $(wildcard src/*.c) $(wildcard src/graphics/*.c) $(wildcard src/maps/*.c)
ASMSOURCES := $(wildcard src/*.s)

all:	$(BINS)

# Compile and link all source files in a single call to LCC
$(BINS):	$(CSOURCES) $(ASMSOURCES)
	$(LCC) -o $@ $(CSOURCES) $(ASMSOURCES)

run:
# sameboy $(BINS)
	bash ~/./retro.sh gearboy $(BINS)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm
