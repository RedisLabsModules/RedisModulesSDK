all: run

SELFDIR := $(dir $(firstword $(MAKEFILE_LIST)))

PYTHON ?= python
TEST_FILTER ?= *

run: $(TEST_OBJS)
	$(PYTHON) "$(SELFDIR)/cunit.py" -f "$(TEST_FILTER)" $(TEST_OBJS)

.PHONY: run