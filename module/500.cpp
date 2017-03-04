#include "../core/HTTPResponse.hpp"
#include "../core/HTTPRequest.hpp"
#include <string>

extern "C" {

extern void getPage(NPPcore::HTTPRequest& request, NPPcore::HTTPResponse& response){
    std::string body = "<h1>Internal Server Error</h1><p>Something went wrong pretty badly.</p>";
    response.setBody(body);
}

}
