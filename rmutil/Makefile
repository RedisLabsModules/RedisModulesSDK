# set environment variable RM_INCLUDE_DIR to the location of redismodule.h
ifndef RM_INCLUDE_DIR
	RM_INCLUDE_DIR=../
endif

CFLAGS ?= -g -fPIC -O3 -std=gnu99 -Wall -Wno-unused-function
CFLAGS += -I$(RM_INCLUDE_DIR)
CC=gcc

OBJS=util.o strings.o sds.o vector.o alloc.o periodic.o

all: librmutil.a

clean:
	rm -rf *.o *.a

librmutil.a: $(OBJS)
	ar rcs $@ $^

test_vector: test_vector.o vector.o
	$(CC) -Wall -o $@ $^ -lc -lpthread -O0
	@(sh -c ./$@)
.PHONY: test_vector

test_periodic: test_periodic.o periodic.o
	$(CC) -Wall -o $@ $^ -lc -lpthread -O0
	@(sh -c ./$@)
.PHONY: test_periodic

export TESTMODULE_SO = rmodule_test.so
export TEST_OBJS = test_string.o test_util.o librmutil.a
export
test_rmodule: librmutil.a $(TEST_OBJS)
	$(MAKE) -f testmodule/Makefile
.PHONY: test_rmodule

test: test_periodic test_vector test_rmodule
.PHONY: test
