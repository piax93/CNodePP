#ifndef MODULELOADER_HPP_
#define MODULELOADER_HPP_

#include "Configuration.hpp"
#include "HTTPResponse.hpp"
#include "NodeppError.hpp"
#include "HTTPRequest.hpp"
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
#define MOD_EXT ".so"
#define MOD_GETPAGE_METHOD "getPage"

namespace NPPcore {

class HTTPResponse;

class ModuleLoader {
private:
	bool online;
	std::thread watcher;
	std::string modfolder;
	std::string defaultmod;
	std::unordered_map<std::string,void*> modules;

	ModuleLoader();
	void loadModule(const std::string& name);
	void deleteModule(const std::string& name);
	void loadAll();
	void watchModuleFolder();
public:
	static ModuleLoader& getInstance() {
		static ModuleLoader instance;
		return instance;
	}
	ModuleLoader(ModuleLoader const&) = delete;
	void operator=(ModuleLoader const&) = delete;
	bool hasModule(const std::string& name) const;
	void* getModule(const std::string& name) const;
	void* getMethod(const std::string& modulename, const std::string& methodname) const;
	void getPage(const std::string& modulename, HTTPRequest& request, HTTPResponse& response);
	virtual ~ModuleLoader();
};

} /* namespace NPPcore */

#endif /* MODULELOADER_HPP_ */
