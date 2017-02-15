CC=g++
CFLAGS = -std=c++1y -rdynamic -O3 -pthread -Wall
MODFLAGS = -std=c++1y -O3 -shared -fPIC -Wall
CLIBS = -ldl

SOURCEDIR=core
INCLUDEDIR=$(SOURCEDIR)
SOURCES=$(wildcard $(SOURCEDIR)/*.cpp)
OBJECTSDIR=bin/obj
OBJECTS=$(patsubst $(SOURCEDIR)/%,$(OBJECTSDIR)/%,$(SOURCES:.cpp=.o))
EXECUTABLEDIR=bin
EXECUTABLE=core
MODSRCDIR=module
MODDIR=$(EXECUTABLEDIR)/module
MODULES=$(wildcard $(MODSRCDIR)/*.cpp)
MODOBJECTS=$(patsubst $(MODSRCDIR)/%,$(MODDIR)/%,$(MODULES:.cpp=.so))
MODALIAS=$(MODULES:.cpp=)
TMPDIR=$(EXECUTABLEDIR)/tmp
DIRS=$(EXECUTABLEDIR) $(OBJECTSDIR) $(MODDIR) $(TMPDIR)


.PHONY: core debug all_modules run clean $(MODALIAS)


core: $(OBJECTS) | $(EXECUTABLEDIR)
	$(CC) $(CFLAGS) -o $(EXECUTABLEDIR)/$(EXECUTABLE) $(OBJECTS) $(CLIBS)


debug: | $(EXECUTABLEDIR)
	$(CC) $(CFLAGS) -g -o $(EXECUTABLEDIR)/$(EXECUTABLE) $(SOURCEDIR)/*.cpp $(CLIBS)


run: core settings.conf
	$(EXECUTABLEDIR)/$(EXECUTABLE)


clean:
	rm -Rf $(EXECUTABLEDIR)


$(DIRS):
	mkdir -p $@


all_modules: $(MODOBJECTS)


.SECONDEXPANSION:
$(OBJECTS): $$(wildcard $$(patsubst $$(OBJECTSDIR)/%.o,$$(SOURCEDIR)/%,$$@).*) | $(OBJECTSDIR)
	$(CC) $(CFLAGS) -c -I $(INCLUDEDIR) $(patsubst %.o,%.cpp,$(patsubst $(OBJECTSDIR)/%,$(SOURCEDIR)/%,$@)) -o $@ $(CLIBS)


$(MODOBJECTS): $$(patsubst %.so,%.cpp,$$(patsubst $$(MODDIR)/%,$$(MODSRCDIR)/%,$$@)) | $(MODDIR) $(TMPDIR)
	$(eval MODNAME := $(notdir $@))
	$(CC) $(MODFLAGS) -o $(TMPDIR)/$(MODNAME) $(MODSRCDIR)/$(MODNAME:.so=.cpp)
	rm -f $@
	cp $(TMPDIR)/$(MODNAME) $(MODDIR)/
	rm -f $($TMPDIR)/$(MODNAME)


$(MODALIAS): $$(patsubst $$(MODSRCDIR)/%,$$(MODDIR)/%.so,$$@)
