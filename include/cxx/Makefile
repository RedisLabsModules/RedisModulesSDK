


CC  := g++
CXX := $(CC)
CPPFLAGS := -I../
CXXFLAGS := -std=c++20 -Wpedantic -Wall -Wextra -Werror -fPIC -g
LDFLAGS := -shared -Wl,-rpath='$$ORIGIN' -L. -g

SRCS := $(wildcard *.cpp)
DEPS := $(patsubst %.cpp, %.d, $(SRCS))
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
LIB  := libmoduleapi.so
LDLIBS := $(patsubst %.o, -l%, $(filter-out moduleapi.o, $(OBJS)))

include $(DEPS)
.DEFAULT_GOAL := $(LIB)

$(DEPS): %.d: %.cpp
	$(CC) $(CPPFLAGS) $< -MMD -MT"$*.o" -MT"$@"

$(OBJS): %.o:
	$(COMPILE.cpp) $< -o $@

$(LIB): $(OBJS)
	$(LINK.o) -o $@ $^ $(LDLIBS)

.PHONY := clean
clean:
	$(RM) *~ $(DEPS) $(OBJS) $(LIB)
