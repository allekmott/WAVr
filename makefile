CC=gcc
CFLAGS=

EXE=wavr

DEPS=sigfapper.h wav.h

OBJ_DEPS=sigfapper.o wavr.o

%.o: $.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ_DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(EXE) *.o *.wav *~
