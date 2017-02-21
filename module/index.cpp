#include "../core/HTTPResponse.hpp"
#include "../core/HTTPRequest.hpp"
#include <unordered_map>
#include <string>

extern "C" {

extern void getPage(NPPcore::HTTPRequest& request, NPPcore::HTTPResponse& response){
	std::unordered_map<std::string,std::string> vars =
		{{"var1", "This is a super long way to say goodbye. {{ var2 }}"}, {"var2", "Supercool"}};
	response.bindTemplate("index.html", vars);
}

}
