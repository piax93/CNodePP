compile_core:
	mkdir -p bin
	g++ -std=c++1y -O3 -pthread -o bin/core core/*.cpp -ldl
compile_modules:
	mkdir -p module/obj
	$(eval temp := $(shell mktemp -d))
	$(foreach name, $(wildcard module/*.cpp), \
    g++ -std=c++1y -O3 -shared -o $(temp)/$(notdir $(basename $(name))).o $(name); \
		cp $(temp)/$(notdir $(basename $(name))).o module/obj/; \
	)
	rm -R $(temp)
run:
	bin/core
