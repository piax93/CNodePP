#include "../core/HTTPResponse.hpp"
#include "../core/HTTPRequest.hpp"

extern "C" {

extern void getPage(NPPcore::HTTPRequest& request, NPPcore::HTTPResponse& response){
	// response.setBody("You won't see me :D");
  response.bindTemplate("test");
}

}
