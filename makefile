compile_core:
	mkdir -p bin
	g++ -std=c++1y -O3 -o bin/core core/*.cpp
run:
	bin/core
