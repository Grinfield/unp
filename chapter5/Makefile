CC = gcc

sub_objs = tcpserver01.o
all_objs :=
all_objs += $(sub_objs) 
all_objs += $(BUILD_DIR)/$(COMMON_OBJS)

.PHONY: all clean

all: tcpserver01

tcpserver01: $(all_objs)
	$(CC) $(SUBDIR_CFLAGS) -o $@ $^

ifneq ($(MAKECMDGOALS),clean)
-include $(sub_objs:.o=.d)
endif

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(SUBDIR_CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# %.o: myunp.h
%.o: %.c
	$(CC) $(SUBDIR_CFLAGS) -o $@ -c $<

clean:
	-rm -f *.o; \
	rm -f *.d; \
	rm -f tcpserver01;
