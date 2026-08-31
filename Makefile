# Warn if gcc version is lower than 4.8 (-Og was introduced in this version)

MIN_GCC_VERSION = "4.8"
GCC_VERSION := "`gcc -dumpversion`"
IS_GCC_ABOVE_MIN_VERSION := $(shell expr "$(GCC_VERSION)" ">=" "$(MIN_GCC_VERSION)")
ifeq "$(IS_GCC_ABOVE_MIN_VERSION)" "1"
	GCC_VERSION_STRING := "GCC version OK $(GCC_VERSION) >= $(MIN_GCC_VERSION)"
else
	GCC_VERSION_STRING := "ERROR: gcc version $(GCC_VERSION) is lower than $(MIN_GCC_VERSION), 'gcc -Og' might fail."
endif


#set environment variable RM_INCLUDE_DIR to the location of redismodule.h
ifndef RM_INCLUDE_DIR
	RM_INCLUDE_DIR=./
endif

ifndef RMUTIL_LIBDIR
	RMUTIL_LIBDIR=rmutil
endif

ifndef SRC_DIR
	SRC_DIR=example
endif


all: module.so

module.so:
	@echo $(GCC_VERSION_STRING)
	$(MAKE) -C ./$(SRC_DIR)
	cp ./$(SRC_DIR)/module.so .

clean: FORCE
	rm -rf *.xo *.so *.o
	rm -rf ./$(SRC_DIR)/*.xo ./$(SRC_DIR)/*.so ./$(SRC_DIR)/*.o
	rm -rf ./$(RMUTIL_LIBDIR)/*.so ./$(RMUTIL_LIBDIR)/*.o ./$(RMUTIL_LIBDIR)/*.a

run:
	redis-server --loadmodule ./module.so

FORCE:
