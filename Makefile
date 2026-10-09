SRCS = $(wildcard src/*.c)

build:
	gcc $(SRCS) -Iinclude `pkg-config --cflags sdl3` `pkg-config --libs sdl3` -o engine -mconsole

run:
	./engine.exe