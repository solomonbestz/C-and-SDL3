build:
	gcc src/main.c `pkg-config --cflags sdl3` `pkg-config --libs sdl3` -o fb -mconsole

run:
	./fb.exe