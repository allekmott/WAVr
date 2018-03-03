CC=gcc
CFLAGS=-Wall -std=c89 -MD

EXE=wavr
DEPS=signal.h wavr.h wav.h
OBJS=signal.o wavr.o wav.o

.PHONY: all
all: $(EXE)

%.o: $.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: rebuild
rebuild: clean all

.PHONY: clean
clean:
	rm -f $(EXE)
	rm -f *.o
	rm -f *.d
	rm -f *.wav
	rm -f *~

-include $(OBJS:.o=.d)

