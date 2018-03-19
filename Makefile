CC = gcc
CFLAGS = -std=gnu99 -Wall -W -O -static -I.
LFLAGS = 

DEPS = send.h receive.h utils.h
OBJ = main.o send.o receive.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

traceroute: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LFLAGS)

clean:
	rm $(OBJ)

distclean:
	rm traceroute $(OBJ)
