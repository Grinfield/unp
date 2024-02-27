CC = gcc

SRC = checkopts.c
OBJ = $(SRC:.c=.o)
EXEC = $(OBJ:%.o=%)

all: $(EXEC)

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
	$(CC) $(LDFLAGS) -o $@ $< $(LBLIBS)

clean:
	-$(RM) *.d; \
	$(RM) $(OBJ) $(EXEC)