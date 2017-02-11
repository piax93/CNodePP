#include "../core/HTTPResponse.hpp"
#include "../core/HTTPRequest.hpp"
#include <string>

extern "C" {

extern void getPage(NPPcore::HTTPRequest& request, NPPcore::HTTPResponse& response){
	std::string body;
	body = "<h1>Forbidden</h1>";
	body += "<p>Access to this resource is forbidden.</p>";
	response.setBody(body);
}

}
