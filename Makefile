SRCS = $(wildcard src/*.c)

build:
	gcc $(SRCS) -Iinclude `pkg-config --cflags sdl3` `pkg-config --libs sdl3` -o fb -mconsole

run:
	./fb.exe