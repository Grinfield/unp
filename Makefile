CC = gcc
CFLAGS = -g

.PHONY: all clean

all: chapter5

chapter5:
	$(MAKE) -C chapter5

clean:
