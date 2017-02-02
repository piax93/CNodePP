#ifndef MODULELOADER_HPP_
#define MODULELOADER_HPP_

#include "Configuration.hpp"
#include "NodeppError.hpp"
#include "utilities.hpp"
#include <unordered_map>
#include <sys/inotify.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <chrono>
#include <thread>

#define EVENT_SIZE (sizeof (struct inotify_event))
#define EVENT_BUF_LEN (128 * (EVENT_SIZE + 16))

namespace NPPcore {

class ModuleLoader {
private:
	bool online;
	std::thread watcher;
	std::unordered_map<std::string,void*> modules;
	static ModuleLoader* self;
	ModuleLoader();
	void loadModule(std::string name);
	void deleteModule(std::string name);
	void reloadModule(std::string name);
	void loadAll();
	static void watchModuleFolder();
public:
	static ModuleLoader* getInstance();
	void* getModule(std::string name) const;
	void* getMethod(std::string modulename, std::string methodname) const;
	virtual ~ModuleLoader();
};

} /* namespace NPPcore */

#endif /* MODULELOADER_HPP_ */