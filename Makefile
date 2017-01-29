CC=gcc
CFLAGS=-I.
DEPS=chip_xio.h
OBJ=chip_xio.o test_chip_xio.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test_chip_xio: $(OBJ)

	$(CC) -o $@ $^ $(CFLAGS)
clean:
	rm -rf ./*.o
