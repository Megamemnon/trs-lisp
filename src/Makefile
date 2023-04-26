CC=gcc
CFLAGS=-I.
DEPS=trs-lisp.h primitives.h
OBJ=trs-lisp.c primitives.c
LIBS=-lgc

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

trs-lisp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o *~ core 