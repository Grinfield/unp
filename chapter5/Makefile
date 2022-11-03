CC = gcc

sub_objs := tcpserver01.o
all_objs :=
all_objs += $(sub_objs) 
all_objs += $(SRC_DIR)/$(COMMON_OBJS)

.PHONY: all clean

all: tcpserver01

tcpserver01: $(all_objs)
	$(CC) $(SUBDIR_CFLAGS) -o $@ $^

ifneq ($(MAKECMDGOALS),clean)
-include $(sub_objs:.o=.d)
endif

%.d: %.c
	@set -e; $(RM) $@; \
	$(CC) -MM $(SUBDIR_CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

%.o: %.c
	$(CC) $(SUBDIR_CFLAGS) -o $@ -c $<

clean:
	-$(RM) *.o; \
	$(RM) *.d; \
	$(RM) tcpserver01;
