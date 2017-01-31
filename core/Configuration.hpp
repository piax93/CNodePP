#ifndef CONFIGURATION_HPP_
#define CONFIGURATION_HPP_

#include "NodeppError.hpp"
#include <unordered_map>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>

namespace NPPcore {

class Configuration {
private:
	std::unordered_map<std::string, std::string> values;
public:
	Configuration(const std::string& filename);
	std::string getValue(const std::string& name) const;
	int getValueToInt(const std::string& name) const;
	friend std::ostream& operator<<(std::ostream &strm, const Configuration& conf);
	virtual ~Configuration();
};

std::ostream& operator<<(std::ostream &strm, const Configuration& conf);

}  /* namespace NPPcore */

#endif /* CONFIGURATION_HPP_ */
