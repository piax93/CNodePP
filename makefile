CFLAGS = -std=c++1y -rdynamic -O3 -pthread
MODFLAGS = -std=c++1y -O3 -shared -fPIC
CLIBS = -ldl


compile_core:
	mkdir -p bin
	g++ $(CFLAGS) -o bin/core core/*.cpp $(CLIBS)

debug:
	mkdir -p bin
	g++ $(CFLAGS) -g -o bin/core core/*.cpp $(CLIBS)

all_modules:
	mkdir -p module/obj
	$(eval temp := $(shell mktemp -d))
	$(foreach name, $(wildcard module/*.cpp), \
    g++ $(MODFLAGS) -o $(temp)/$(notdir $(basename $(name))).so $(name); \
		rm -f module/obj/$(notdir $(basename $(name))).so; \
		cp $(temp)/$(notdir $(basename $(name))).so module/obj/; \
	)
	rm -R $(temp)

compile_module:
ifdef MOD
	mkdir -p module/obj
	$(eval temp := $(shell mktemp -d))
	g++ $(MODFLAGS) -o $(temp)/$(MOD).so module/$(MOD).cpp
	rm -f module/obj/$(MOD).so
	cp $(temp)/$(MOD).so module/obj/
	rm -R $(temp)
else
	echo "Usage: make module MOD=<module_name>"
endif

run:
	bin/core
