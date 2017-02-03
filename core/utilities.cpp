#include "utilities.hpp"

namespace util {

std::vector<std::string> splitString(const std::string& src, const std::string& delimiter){
	std::vector<std::string> res;
	std::string tmp(src);
	size_t pos = 0;
	while(true){
		pos = tmp.find(delimiter);
		if(pos == std::string::npos) {
			res.push_back(tmp);
			break;
		}
		res.push_back(tmp.substr(0, pos));
		tmp = tmp.substr(pos+delimiter.length());
	}
	return res;
}

std::vector<std::string> splitString(char* src, char delimiter){
	std::vector<std::string> res;
	size_t len = strlen(src);
	char* pos;
	char* tmp = src;
	while(true){
		pos = strchr(tmp, delimiter);
		if(pos == NULL) {
			res.push_back(std::string(tmp));
			break;
		}
		*pos = '\0';
		res.push_back(std::string(tmp));
		*pos = delimiter;
		tmp = pos + 1;
	}
	return res;
}

bool endsWith(const std::string& value, const std::string& ending) {
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool endsWith(const char* value, const std::string& ending) {
	size_t lenv = strlen(value);
	size_t lene = ending.length();
	if (lene > lenv) return false;
	for(size_t i = 1; i <= lene; i++) {
		if(value[lenv-i] != ending[lene-i]) return false;
	}
	return true;
}

std::string removeExtension(const char* filename){
	const char* dot = strrchr(filename, '.');
	return std::string(filename, dot-filename);
}

std::string removeExtension(const std::string& filename){
	size_t pos = filename.find_last_of('.');
	return filename.substr(0, pos);
}

}  /* namespace util */
