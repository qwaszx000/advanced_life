build:
	g++ -I/usr/include/gtk-3.0 src/*.cpp src/gui/*.hpp -o simulation.exe `pkg-config gtkmm-3.0 --cflags --libs`

run:
	./simulation.exe

build_run: build run