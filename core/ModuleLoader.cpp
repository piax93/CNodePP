#include "ModuleLoader.hpp"

namespace NPPcore {

ModuleLoader* ModuleLoader::self = NULL;

ModuleLoader::ModuleLoader() {
	online = true;
	loadAll();
	watcher = std::thread(watchModuleFolder);
}

void ModuleLoader::watchModuleFolder(){
	int inot, wd;
	size_t len, i;
	char lastfile[NAME_MAX];
	char buffer[EVENT_BUF_LEN];
	struct inotify_event* event;
	inot = inotify_init();
	if(inot < 0) {
		std::cerr << "Fatal: inotify_init() failed" << std::endl;
		exit(-1);
	}
	wd = inotify_add_watch(inot, Configuration::self.getValue("mod_dir").c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
	while(self->online){
		len = read(inot, buffer, EVENT_BUF_LEN);
		if(len < 0) throw NodeppError("inotify error");
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		i = 0;
		while(i < len){
			event = (struct inotify_event*) &buffer[i];
			if(!(event->mask & IN_ISDIR)){
				if (event->mask & IN_CREATE){
					std::cout << "Create" << std::endl;
				} else if (event->mask & IN_DELETE) {
					std::cout << "Del" << std::endl;
				} else if ((event->mask & IN_MODIFY) && (strcmp(lastfile, event->name) != 0)) {
					std::cout << "Mod:'" << event->name << "'" << std::endl;
				}
			}
			stpcpy(lastfile, event->name);
			i += EVENT_SIZE + event->len;
		}
	}
}

void ModuleLoader::loadModule(std::string name){
	void* handle = dlopen((Configuration::self.getValue("mod_dir")+"/"+name+".o").c_str(), RTLD_LAZY);
	if(!handle) throw NodeppError("Cannot load module " + name);
	dlerror();
	modules[name] = handle;
	std::cout << "Loaded " << name << std::endl;
}

void ModuleLoader::reloadModule(std::string name){
	dlclose(getModule(name));
	loadModule(name);
}

void ModuleLoader::deleteModule(std::string name){
	dlclose(getModule(name));
	modules.erase(name);
}

void ModuleLoader::loadAll(){
	struct dirent* ent;
	DIR* dir = opendir(Configuration::self.getValue("mod_dir").c_str());
	std::string filename;
	if(dir == NULL) {
		std::cerr << "Fatal: cannot open module directory";
		exit(-1);
	}
	while((ent = readdir(dir)) != NULL){
		filename = std::string(ent->d_name);
		if(util::ends_with(filename, ".o")){
			loadModule(filename.substr(0, filename.length()-2));
		}
	}
	closedir(dir);
}

void* ModuleLoader::getModule(std::string name) const {
	std::unordered_map<std::string,void*>::const_iterator it = modules.find(name);
	if(it == modules.end()) throw NodeppError("Module " + name + " was not loaded");
	return it->second;
}

void* ModuleLoader::getMethod(std::string modulename, std::string methodname) const{
	void* handle = getModule(methodname);
	void* method = dlsym(handle, methodname.c_str());
	const char* dlsym_err = dlerror();
	if(dlsym_err) throw NodeppError("Cannot load method " + methodname + " from " + modulename);
	return method;
}

ModuleLoader* ModuleLoader::getInstance(){
	if(self == NULL) self = new ModuleLoader();
	return self;
}

ModuleLoader::~ModuleLoader() {
	std::unordered_map<std::string,void*>::const_iterator it;
	for(it = modules.begin(); it != modules.end(); it++) dlclose(it->second);
	online = false;
	watcher.join();
}

} /* namespace NPPcore */
