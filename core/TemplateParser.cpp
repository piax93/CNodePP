#include "TemplateParser.hpp"
#include <iostream>

namespace NPPcore {

std::string TemplateParser::parse(const std::string& tplname,
		const std::unordered_map<std::string, std::string>& variables, HTTPRequest& request){

	std::string filename = Configuration::self.getValue("template_dir") + "/" + tplname + ".html";
	std::string content = util::readFileToString(filename);
	std::regex var_pattern = std::regex("\\{\\{\\s*([a-zA-Z0-9_]+)\\s*\\}\\}");
	std::regex_iterator<std::string::iterator> it = std::regex_iterator<std::string::iterator>(content.begin(), content.end(), var_pattern);
	std::regex_iterator<std::string::iterator> itend;
	while(it != itend){
		auto varvalue = variables.find((*it)[1]);
		if(varvalue != variables.end()) content.replace(it->position(), it->length(), varvalue->second);
		it++;
	}
	return content;
}

} /* namespace NPPcore */
