#ifndef HTTPREQUEST_HPP_
#define HTTPREQUEST_HPP_

#include "NodeppError.hpp"
#include "utilities.hpp"
#include <unordered_map>
#include <iostream>
#include <string.h>
#include <string>

#define MAX_OPTION_LEN 8192
#define DEFAULT_ROUTE "index"

namespace NPPcore {

enum REQUEST_TYPE {
	GET, HEAD, POST, PUT
};

class HTTPRequest {
private:
	REQUEST_TYPE requestType;
	std::string version;
	std::string route;
	std::unordered_map<std::string, std::string> urlparams;
	std::unordered_map<std::string, std::string> options;
public:
	HTTPRequest() : requestType(GET) {}
	void load(FILE* socket_pointer);
	std::string getVersion() const;
	std::string getRoute() const;
	REQUEST_TYPE getType() const;
	std::string getOption(const std::string& name) const;
	friend std::ostream& operator<<(std::ostream &strm, const HTTPRequest& req);
	virtual ~HTTPRequest() {}
};

std::ostream& operator<<(std::ostream &strm, const HTTPRequest& req);

} /* namespace NPPcore */

#endif /* HTTPREQUEST_HPP_ */
