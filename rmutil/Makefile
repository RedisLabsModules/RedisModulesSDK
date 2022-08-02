
GCC_FLAGS ?= -g -fPIC -O3 -std=gnu99 -Wall -Wno-unused-function -I../include
GCC=gcc

OBJS=util.o strings.o sds.o vector.o alloc.o periodic.o

all: librmutil.a

clean:
	rm -rf *.o *.a

%.o : %.c
	$(GCC) -c $(GCC_FLAGS) -Wall -o $@ $^ -O0

librmutil.a: $(OBJS)
	ar rcs $@ $^

test_vector: test_vector.o vector.o
	$(GCC) $(GCC_FLAGS) -Wall -o $@ $^ -lc -lpthread -O0
	./$@

test_periodic: test_periodic.o periodic.o
	$(CC) -Wall -o $@ $^ -lc -lpthread -O0
	./$@

.PHONY: test_vector test_periodic
	
test: test_periodic test_vector

.PHONY: test
