CC=gcc
CFLAGS=-lm

EXE=wavr

DEPS=signal.h wavr.h wav.h

OBJ_DEPS=signal.o wavr.o wav.o

%.o: $.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ_DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(EXE) *.o *.wav *~
