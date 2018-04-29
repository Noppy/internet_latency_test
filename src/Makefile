CC      = gcc
CFLAGS  = -Wall -g
LDLIBS  =
PROGRAM = echo_client echo_server throughput_server throughput_client

all: $(PROGRAM)

echo_server: echo_server.o
	$(CC) -o $@ $^

echo_client: echo_client.o
	$(CC) -o $@ $^

throughput_server: throughput_server.o
	$(CC) -o $@ $^

throughput_client: throughput_client.o
	$(CC) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) $(LDLIBS) -c $<

.PHONY: clean
clean:
	rm -rf $(PROGRAM) *.o


