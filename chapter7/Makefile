CC = gcc

SRC = checkopts.c tcpcliopts.c
OBJ = $(SRC:.c=.o)
EXEC = $(OBJ:%.o=%)

all: $(EXEC)

ifeq ($(COMMON_OBJS),)
	$(error 'Variable COMMON_OBJS undefined.')
endif

ifneq ($(MAKECMDGOALS),clean)
-include $(OBJ:.o=.d)
endif

%.d: %.c
	@set -e; $(RM) $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(EXEC): % : %.o
	$(CC) $(LDFLAGS) -o $@ $< $(COMMON_OBJS)

clean:
	-$(RM) *.d; \
	$(RM) $(OBJ) $(EXEC)