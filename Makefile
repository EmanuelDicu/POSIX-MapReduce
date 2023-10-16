CC = gcc

CFLAGS = -Wall -Werror -I.

DEPS = data_types.h handle_error.h map_implement.h\
map_reduce.h numeric_helpers.h reduce_helpers.h reduce_implement.h

OBJ = main.o data_types.o handle_error.o map_implement.o\
map_reduce.o numeric_helpers.o reduce_helpers.o reduce_implement.o

LIBS = -lm -lpthread


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

build: $(OBJ)
	$(CC) -o mapreduce $^ $(CFLAGS) $(LIBS)

clean:
	rm mapreduce *.o

.PHONY: clean

