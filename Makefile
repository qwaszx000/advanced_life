build:
	g++ -I/usr/include/gtk-3.0 `find -regextype egrep -regex ".*\.(hpp|cpp)"` -o simulation.exe `pkg-config gtkmm-3.0 --cflags --libs`

run:
	./simulation.exe

build_run: build run