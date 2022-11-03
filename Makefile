CC = gcc
CFLAGS = -g -O0
SRC_DIR = $(CURDIR)

CFLAGS += -I$(SRC_DIR)/include

SUBDIR_CFLAGS = -g -O0 
SUBDIR_CFLAGS += -I$(SRC_DIR)/include

TARGET_DIRS = chapter5
SUBDIR_RULES = $(patsubst %,subdir-%, $(TARGET_DIRS))

COMMON_OBJS := common/wraps.o
LIBS :=

.PHONY: all clean

all: $(SUBDIR_RULES)

ifneq ($(MAKECMDGOALS),clean)
-include $(COMMON_OBJS:.o=.d)
endif

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(SUBDIR_CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(COMMON_OBJS):

subdir-%: $(COMMON_OBJS)
	$(MAKE) -C $* SUBDIR_CFLAGS="$(SUBDIR_CFLAGS)" COMMON_OBJS="$(COMMON_OBJS)" SRC_DIR=$(SRC_DIR) all

clean:
	-$(RM) common/*.o; \
	$(RM) common/*.d; \
	for d in $(TARGET_DIRS); do \
	if test -d $$d; then $(MAKE) -C $$d $@ || exit 1; fi; \
        done