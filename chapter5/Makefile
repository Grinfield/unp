CC = gcc
CFLAGS = -g -O0 -I../include
objects = tcpserver01.o wraps.o

.PHONY: all clean

all: tcpserver01

tcpserver01: tcpserver01.o wraps.o
	$(CC) $(CFLAGS) -o $@ $^

ifneq ($(MAKECMDGOALS),clean)
-include $(objects:.o=.d)
endif

$(objects):

%.d: %.c sub-x86_soft
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# %.o: myunp.h
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

sub-%:
	echo "$*"

clean:
	-rm -f *.o; \
	rm -f *.d; \
	rm -f tcpserver01;
