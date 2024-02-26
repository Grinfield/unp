CC = gcc

sub_objs := tcpserver01.o tcpcli01.o tcpcli04.o
all_objs := $(sub_objs:%.o=%)

.PHONY: all clean

all: $(all_objs)

ifeq ($(COMMON_OBJS),)
	$(error 'Variable COMMON_OBJS undefined.')
endif

ifneq ($(MAKECMDGOALS),clean)
-include $(sub_objs:.o=.d)
endif

%.d: %.c
	@set -e; $(RM) $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(all_objs): $(COMMON_OBJS)

$(all_objs): % : %.o
	$(CC) $(CFLAGS) -o $@ $< $(COMMON_OBJS)

clean:
	-$(RM) *.o; \
	$(RM) *.d; \
	$(RM) tcpserver01; \
	$(RM) tcpcli01; \
	$(RM) tcpcli04
