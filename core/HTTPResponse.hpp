#ifndef HTTPRESPONSE_HPP_
#define HTTPRESPONSE_HPP_

#include "Configuration.hpp"
#include "ModuleLoader.hpp"
#include "NodeppError.hpp"
#include "HTTPRequest.hpp"
#include <unordered_map>
#include "strutils.hpp"
#include "netutils.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
#include <regex>

#define MAX_RECURSION 20

namespace NPPcore {

class HTTPResponse {
private:
	int code;
	HTTPRequest& request;
	size_t bindCount;
	std::string body;
	std::string staticFilename;
	std::unordered_map<std::string,std::string> options;
public:
	const static std::unordered_map<int,std::string> codes;
	HTTPResponse(HTTPRequest& request, int code);
	void send(int socketfd);
	void setCode(int code);
	void setOption(const std::string& name, const std::string& value);
	void setStaticFile(const std::string& fileroute);
	void setBody(const std::string& body);
	void appendBody(const std::string& part);
	void bindTemplate(const std::string& tplname, bool statictpl = false);
	void bindTemplate(const std::string& tplname, const std::unordered_map<std::string, std::string>& variables, bool statictpl = false);
	int getCode() const;
	std::string getOption(const std::string& name) const;
	std::string getBody() const;
	friend std::ostream& operator<<(std::ostream &strm, const HTTPResponse& resp);
	virtual ~HTTPResponse();
};

std::ostream& operator<<(std::ostream &strm, const HTTPResponse& resp);

} /* namespace NPPcore */

#endif /* HTTPRESPONSE_HPP_ */
