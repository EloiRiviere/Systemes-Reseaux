export CC = gcc
export CFLAGS = -Wall

all: recv send

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%: %.o
	$(CC) $^ -o $@

recv: recv.o
send: send.o

clean:
	rm -f *.o send recv
