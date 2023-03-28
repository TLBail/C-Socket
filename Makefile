CC := gcc
CFLAGS := -Wall -Werror -Wextra

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=object/%.o)
BUILD := $(SRCS:src/%.c=build/%.out)

.PHONY: all clean run

all: $(OBJS) $(BUILD)

object/%.o: src/%.c
	@mkdir -p $(@D)

	$(CC) -c $^ $(CFLAGS) -o $@

build/%.out: object/%.o
	@mkdir -p $(@D)

	$(CC) $^ $(CFLAGS) -o $@

clean:
	$(RM) -rf object
	$(RM) -rf build

run:
	tmux new-session -d -s mysession './build/socket.out 8000 8001'
	tmux split-window -v './build/socket.out 8003 8000'
	tmux select-pane -t 0
	tmux split-window -h './build/socket.out 8001 8002'
	tmux select-pane -t 2
	tmux split-window -h './build/client.out 8000 && ./build/socket.out 8002 8003'
	tmux attach-session -t mysession
	