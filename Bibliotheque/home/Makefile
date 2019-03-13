CC = gcc
CFLAGS = -Wall
INCLUDE = -I /tmp/graphic/usr/include/
LIBRARY = -L /tmp/graphic/usr/lib/
LIB = -lX11 -lgraphic -lpthread -lrt
EXECUTABLE = test

all: $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -pthread -c $< $(INCLUDE)

%: %.o
	$(CC) $^ -o $@ $(LIB) $(LIBRARY)

test: test.o
test.o:test.c

clean:
	rm -f *.o $(EXECUTABLE)
