CC = gcc
CFLAGS = -g

BUILD_DIR = $(shell pwd)
TARGET_DIRS = chapter5
SUBDIR_RULES = $(patsubst %,subdir-%, $(TARGET_DIRS))

.PHONY: all clean

all: $(SUBDIR_RULES)

subdir-%:
	$(MAKE) -C $* all

clean:
	for d in $(TARGET_DIRS); do \
	if test -d $$d; then $(MAKE) -C $$d $@ || exit 1; fi; \
        done