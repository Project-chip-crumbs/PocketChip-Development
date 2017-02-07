CC=gcc
CFLAGS=-I.
DEPS=chip_xio.h
OBJ1=chip_xio.o 
OBJ2=test_chip_xio.o $(OBJ1)
OBJ3=input.o $(OBJ1)
OBJ4=blink.o $(OBJ1)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: test_chip_xio test_input test_blink clean

test_chip_xio: $(OBJ2)

	$(CC) -o $@ $^ $(CFLAGS)

test_input: $(OBJ3)
	$(CC) -o $@ $^ $(CFLAGS)


test_blink: $(OBJ4)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf ./*.o
