#ifndef CONFIGURATION_HPP_
#define CONFIGURATION_HPP_

#include "NodeppError.hpp"
#include "strutils.hpp"
#include <unordered_map>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>

#define MBYTE(x) ((size_t)x * 1048576)
#define CONF_DEFAULT_FILE "settings.conf"
#define CONF_ASSIGN_SYMBOL '='
#define CONF_COMMENT '#'

namespace NPPcore {

class Configuration {
private:
    static std::string file;
    std::unordered_map<std::string, std::string> values;
    Configuration(const std::string& filename);
public:
    static Configuration& getInstance() {
        static Configuration self(file.length() == 0 ? CONF_DEFAULT_FILE : file);
        return self;
    }
    Configuration(Configuration const&) = delete;
    void operator=(Configuration const&) = delete;
    static void setup(char* filename);
    std::string getValue(const std::string& name) const;
    int getValueToInt(const std::string& name) const;
    friend std::ostream& operator<<(std::ostream &strm, const Configuration& conf);
    virtual ~Configuration() {};
};

std::ostream& operator<<(std::ostream &strm, const Configuration& conf);

}  /* namespace NPPcore */

#endif /* CONFIGURATION_HPP_ */
