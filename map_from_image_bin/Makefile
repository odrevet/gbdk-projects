GBDK_HOME = ../../gbdk/

LCC = $(GBDK_HOME)bin/lcc
# LCC = $(LCC) -debug

PROJECTNAME = demo_map
BINS	    = $(PROJECTNAME).gb
CSOURCES   := $(wildcard src/*.c)

all:	$(BINS)

# Compile and link all source files in a single call to LCC
$(BINS):	$(CSOURCES) $(ASMSOURCES)
	$(LCC)  -o $@ $(CSOURCES) $(ASMSOURCES)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm
