CC=gcc
CFLAGS=-I.
DEPS=chip_xio.h
OBJ1=chip_xio.o 
OBJ2=test_chip_xio.o $(OBJ1)
OBJ3=input.o $(OBJ1)
OBJ4=blink.o $(OBJ1)
OBJ5=ff_cycle.o $(OBJ1)
OBJ6=xio_thread.o $(OBJ1)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: test_chip_xio test_input test_blink ff_cycle xio_thread clean

test_chip_xio: $(OBJ2)
	$(CC) -o $@ $^ $(CFLAGS)

test_input: $(OBJ3)
	$(CC) -o $@ $^ $(CFLAGS)

test_blink: $(OBJ4)
	$(CC) -o $@ $^ $(CFLAGS)

ff_cycle: $(OBJ5)
	$(CC) -o $@ $^ $(CFLAGS) -lpthread

xio_thread: $(OBJ6)
	$(CC) -o $@ $^ $(CFLAGS) -lpthread

clean:
	rm -rf ./*.o
