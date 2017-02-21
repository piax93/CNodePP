#include "Configuration.hpp"

namespace NPPcore {

std::string Configuration::file = "";

Configuration::Configuration(const std::string& filename) {
	std::ifstream fs;
	std::string line, name, value;
	fs.open(filename);
	while(std::getline(fs, line)) {
		if(line.length() == 0 || util::trim(line)[0] == CONF_COMMENT) continue;
		size_t pos = line.find(CONF_ASSIGN_SYMBOL);
		if(pos == line.npos) continue;
		name = line.substr(0, pos);
		value = line.substr(pos+1, line.length()-pos-1);
		values[util::trim(name)] = util::trim(value);
	}
	fs.close();
}

void Configuration::setup(char* filename){
	file = std::string(filename);
}

std::string Configuration::getValue(const std::string& name) const {
	std::unordered_map<std::string,std::string>::const_iterator it = values.find(name);
	if(it == values.end()) throw NPPcore::NodeppError("Parameter " + name + " is missing from configuration");
	return it->second;
}

int Configuration::getValueToInt(const std::string& name) const {
	return std::stoi(getValue(name));
}

std::ostream& operator<<(std::ostream &strm, const Configuration& conf) {
	for(auto it = conf.values.begin(); it != conf.values.end(); it++){
		strm << it->first << " -> " << it->second << std::endl;
	}
	return strm;
}

}  /* namespace NPPcore */
