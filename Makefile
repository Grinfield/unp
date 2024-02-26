export SRC_DIR = $(CURDIR)

CC = gcc
export CFLAGS = -g -O0
CFLAGS += -I$(SRC_DIR)/include

TARGET_DIRS = chapter5 chapter6
SUBDIR_RULES = $(patsubst %,subdir-%, $(TARGET_DIRS))

export COMMON_OBJS := $(SRC_DIR)/common/wraps.o $(SRC_DIR)/common/strcliselect01.o
LIBS :=

.PHONY: all clean

all: $(SUBDIR_RULES)

ifneq ($(MAKECMDGOALS),clean)
-include $(COMMON_OBJS:.o=.d)
endif

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($(*F)\)\.o[ :]*,\1.o $(@F) : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

subdir-%: $(COMMON_OBJS)
	$(MAKE) -C $* all

clean:
	-$(RM) common/*.o; \
	$(RM) common/*.d; \
	for d in $(TARGET_DIRS); do \
	if test -d $$d; then $(MAKE) -C $$d $@ || exit 1; fi; \
        done