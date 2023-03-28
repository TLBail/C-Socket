CC := gcc
CFLAGS := -Wall -Werror -Wextra

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=object/%.o)
BUILD := $(SRCS:src/%.c=build/%.out)

.PHONY: all clean

all: $(OBJS) $(BUILD)

object/%.o: src/%.c
	@mkdir -p $(@D)

	$(CC) -c $^ $(CFLAGS) -o $@

build/%.out: object/%.o
	@mkdir -p $(@D)

	$(CC) $^ $(CFLAGS) -o $@

clean:
	$(RM) -rf object