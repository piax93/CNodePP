#include "../core/HTTPResponse.hpp"
#include "../core/HTTPRequest.hpp"
#include <string>

extern "C" {

extern void getPage(NPPcore::HTTPRequest& request, NPPcore::HTTPResponse& response){
	std::string body;
	body = "<h1>Not Found!</h1>";
	body += "<p>The requested URL was not found on this server.</p>";
	response.setBody(body);
}

}
