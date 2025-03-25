CC = gcc
CFLAGS = -O2 -std=c11 -pedantic -Wall -Wextra -Werror -D_DEFAULT_SOURCE

_SRC = app.c consumer.c setup_teardown.c prodcons_queue.c producer.c
SRC = $(patsubst %,src/%,$(_SRC))

ipc: $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: run
run:
	make ipc
	./ipc
	make clean

.PHONY: clean
clean:
	rm ipc