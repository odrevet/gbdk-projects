LCC = $(GBDK_HOME)/bin/lcc
PROJECTNAME = image
BINS	    = $(PROJECTNAME).gb
CSOURCES   := $(wildcard src/*.c)
LCCFLAGS += -Wl-yt0x1B -Wl-j -Wm-yoA -Wm-ya4 -autobank -Wb-ext=.rel -Wb-v

all:	$(BINS)

# Compile and link all source files in a single call to LCC
$(BINS):	$(CSOURCES) $(ASMSOURCES)
	$(LCC) $(LCCFLAGS) -o $@ $(CSOURCES) $(ASMSOURCES)

run:
# sameboy $(BINS)
	bash ~/./retro.sh gearboy $(BINS)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm
	rm -f src/image*
	rm -f assets/gb/*
