
RMUTIL_LIBDIR ?= ../rmutil

# find the OS
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

# Compile flags for linux / osx
ifeq ($(uname_S),Linux)
SHOBJ_CFLAGS ?= -fno-common -g -ggdb
SHOBJ_LDFLAGS ?= -shared -Bsymbolic
else
SHOBJ_CFLAGS ?= -dynamic -fno-common -g -ggdb
SHOBJ_LDFLAGS ?= -bundle -undefined dynamic_lookup
endif

GCC_FLAGS = -I.. -I../include -Wall -g -fPIC -lc -lm -std=gnu99
GCC=gcc

GXX_FLAGS = -I.. -I../include -Wall -g -fPIC -lc -lm
GXX=g++

all: rmutil module.so module-cxx.so

rmutil: $(RMUTIL_LIBDIR)/librmutil.a
	$(MAKE) -C $(RMUTIL_LIBDIR)

%.o : %.c
	$(GCC) -c $(GCC_FLAGS) -Wall -o $@ $^

%.o : %.cc
	$(GXX) -c $(GXX_FLAGS) -Wall -o $@ $^

module.so: module.o
	$(GCC) -o $@ $^ $(GCC_FLAGS) $(SHOBJ_LDFLAGS) $(LIBS) -L$(RMUTIL_LIBDIR) -lrmutil -lc 

module-cxx.so: module-cxx.o
	$(GXX) -o $@ $^ $(GXX_FLAGS) $(SHOBJ_LDFLAGS) $(LIBS) -L$(RMUTIL_LIBDIR) -lrmutil -lc

clean:
	rm -rf *.xo *.so *.o

FORCE:
