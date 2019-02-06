CC = gcc
CFLAGS = -Wall
INCLUDE = -I /tmp/graphic/usr/include/
LIBRARY = -L /tmp/graphic/usr/lib/
LIB = -lX11 -lgraphic
EXECUTABLE = test

all: $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(INCLUDE)

%: %.o
	$(CC) $^ -o $@ $(LIB) $(LIBRARY)

test: test.o
test.o:test.c

clean:
	rm -f *.o $(EXECUTABLE)
