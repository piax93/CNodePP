#include "Configuration.hpp"

namespace NPPcore {

const Configuration Configuration::self(CONF_FILE);

Configuration::Configuration(const std::string& filename) {
	std::ifstream fs;
	std::string line;
	fs.open(filename);
	while(std::getline(fs, line)) {
		size_t pos = line.find('=');
		if(line.length() == 0 || pos == line.npos) continue;
		values[line.substr(0, pos)] = line.substr(pos+1, line.length()-pos-1);
	}
	fs.close();
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
	std::unordered_map<std::string,std::string>::const_iterator it;
	for(it = conf.values.begin(); it != conf.values.end(); it++){
		strm << it->first << " -> " << it->second << std::endl;
	}
	return strm;
}

Configuration::~Configuration() { }

}  /* namespace NPPcore */
