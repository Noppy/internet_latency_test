CC      = gcc
CFLAGS  = -Wall -g
LDLIBS  =
PROGRAM = echo_client echo_server

all: echo_client echo_server

echo_server: echo_server.o
	$(CC) -o $@ $^

echo_client: echo_client.o
	$(CC) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) $(LDLIBS) -c $<

.PHONY: clean
clean:
	rm -rf echo_client echo_server *.o


