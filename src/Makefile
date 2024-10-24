CFLAGS = -Wall -g -std=c11
LDLIBS = -lpulse-simple
BINS = recorder player

all: ${BINS}

recorder: wav.o

player: wav.o

clean:
	rm -f $(BINS)
	rm -f *.o
