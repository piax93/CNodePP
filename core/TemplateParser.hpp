#ifndef TEMPLATEPARSER_HPP_
#define TEMPLATEPARSER_HPP_

#include "Configuration.hpp"
#include "HTTPRequest.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <regex>

namespace NPPcore {

class TemplateParser {
public:
	static std::string parse(const std::string& tplname, const std::unordered_map<std::string, std::string>& variables, HTTPRequest& request);
};

} /* namespace NPPcore */

#endif /* TEMPLATEPARSER_HPP_ */
