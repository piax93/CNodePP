#include "ModuleLoader.hpp"

namespace NPPcore {

ModuleLoader::ModuleLoader() {
	Configuration& conf = Configuration::getInstance();
	online = true;
	modfolder = conf.getValue("mod_dir");
	defaultmod = conf.getValue("mod_default");
	loadAll();
	watcher = std::thread(&ModuleLoader::watchModuleFolder, this);
}

void ModuleLoader::watchModuleFolder(){
	int inot, wd;
	size_t len, i;
	char buffer[EVENT_BUF_LEN];
	struct inotify_event* event;
	inot = inotify_init();
	if(inot < 0) die("Fatal: inotify_init() failed");
	wd = inotify_add_watch(inot, modfolder.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
	if(wd < 0) die("Error adding watcher to module folder");
	while(online){
		len = read(inot, buffer, EVENT_BUF_LEN);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if(len < 0) throw NodeppError("inotify error");
		i = 0;
		while(i < len){
			event = (struct inotify_event*) &buffer[i];
			if(!(event->mask & IN_ISDIR) && util::endsWith(event->name, MOD_EXT)){
				if (event->mask & IN_CREATE){
					// std::cerr << "Created module: " << event->name << std::endl;
					loadModule(util::removeExtension(event->name));
				} else if (event->mask & IN_DELETE) {
					// std::cerr << "Deleted module: " << event->name << std::endl;
					deleteModule(util::removeExtension(event->name));
				}
			}
			i += EVENT_SIZE + event->len;
		}
	}
}

void ModuleLoader::loadModule(const std::string& name){
	void* handle = dlopen((modfolder+"/"+name+MOD_EXT).c_str(), RTLD_NOW);
	if(!handle) throw NodeppError("Cannot load module " + name + ": " + std::string(dlerror()));
	dlerror();
	modules[name] = handle;
	std::cerr << "Loaded " << name << std::endl;
}

void ModuleLoader::deleteModule(const std::string& name){
	dlclose(getModule(name));
	modules.erase(name);
}

void ModuleLoader::loadAll(){
	struct dirent* ent;
	DIR* dir = opendir(modfolder.c_str());
	std::string filename;
	if(dir == NULL) die("Fatal: cannot open module directory");
	while((ent = readdir(dir)) != NULL){
		filename = std::string(ent->d_name);
		if(util::endsWith(filename, MOD_EXT)){
			loadModule(util::removeExtension(filename));
		}
	}
	closedir(dir);
}

bool ModuleLoader::hasModule(const std::string& name) const {
	if(name.length() == 0) return true;
	return modules.find(name) != modules.end();
}

void* ModuleLoader::getModule(const std::string& name) const {
	std::unordered_map<std::string,void*>::const_iterator it = modules.find(name);
	if(it == modules.end()) throw NodeppError("Module " + name + " was not loaded");
	return it->second;
}

void* ModuleLoader::getMethod(const std::string& modulename, const std::string& methodname) const {
	void* handle = getModule(modulename.length() == 0 ? defaultmod : modulename);
	void* method = dlsym(handle, methodname.c_str());
	const char* dlsym_err = dlerror();
	if(dlsym_err) throw NodeppError("Cannot load method " + methodname + " from " + modulename);
	return method;
}

void ModuleLoader::getPage(const std::string& modulename, HTTPRequest& request, HTTPResponse& response){
	if(hasModule(modulename)) {
		getPage_t getPage = (getPage_t)getMethod(modulename, MOD_GETPAGE_METHOD);
		getPage(request, response);
	}
}

ModuleLoader::~ModuleLoader() {
	for(auto it = modules.begin(); it != modules.end(); it++) dlclose(it->second);
	online = false;
	std::ofstream ofs;
	std::string dummy = modfolder + "/.enddummy";
	ofs.open(dummy);
	ofs << '1';
	ofs.close();
	remove(dummy.c_str());
	watcher.join();
	std::cerr << "ModuleLoader freed" << std::endl;
}

} /* namespace NPPcore */
