CC=gcc
CFLAGS=-g -I.
DEPS=src/trs-lisp.h src/primitives.h
OBJ=src/trs-lisp.c src/primitives.c
LIBS=-lgc

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

trs-lisp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o *~ core 