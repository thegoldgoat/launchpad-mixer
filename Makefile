all: launchpad-mixer

WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2
CCX = g++
LIBS = -lrtmidi -lasound

launchpad-mixer: Makefile launchpad-mixer.c
	$(CCX) -o $@ $(WARNINGS) $(DEBUG) $(OPTIMIZE) $(LIBS) launchpad-mixer.c

clean:
	rm -f launchpad-mixer

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./launchpad-mixer

