#include "../core/HTTPResponse.hpp"
#include "../core/HTTPRequest.hpp"
#include <string>

extern "C" {

extern void getPage(NPPcore::HTTPRequest& request, NPPcore::HTTPResponse& response){
	response.setBody("<h1>Che bel body!!!</h1>");
}

}
