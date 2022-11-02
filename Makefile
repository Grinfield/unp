CC = gcc
CFLAGS = -g -O0
BUILD_DIR = $(CURDIR)

CFLAGS += -I$(BUILD_DIR)/include

SUBDIR_CFLAGS = -g -O0 
SUBDIR_CFLAGS += -I$(BUILD_DIR)/include

TARGET_DIRS = chapter5
SUBDIR_RULES = $(patsubst %,subdir-%, $(TARGET_DIRS))

common_objs := common/wraps.o

.PHONY: all clean

all: $(SUBDIR_RULES)

ifneq ($(MAKECMDGOALS),clean)
-include $(common_objs:.o=.d)
endif

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(SUBDIR_CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(common_objs):

subdir-%: $(common_objs)
	$(MAKE) -C $* SUBDIR_CFLAGS="$(SUBDIR_CFLAGS)" COMMON_OBJS="$(common_objs)" BUILD_DIR=$(BUILD_DIR) all

clean:
	-rm -f common/*.o; \
	rm -f common/*.d; \
	for d in $(TARGET_DIRS); do \
	if test -d $$d; then $(MAKE) -C $$d $@ || exit 1; fi; \
        done