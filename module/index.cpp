#include "../core/HTTPResponse.hpp"
#include "../core/HTTPRequest.hpp"
#include <unordered_map>
#include <string>

extern "C" {

extern void getPage(NPPcore::HTTPRequest& request, NPPcore::HTTPResponse& response){
	std::unordered_map<std::string,std::string> vars;
	vars["var1"] = "This is a super long way to say goodbye.";
	vars["var2"] = "Supercool";
	vars["get"] = request.$GET("text");
	vars["post"] = request.$POST("t");
	response.bindTemplate("index.html", vars);
}

}
