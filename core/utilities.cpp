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

}  /* namespace util */
