#ifndef HTTPRESPONSE_HPP_
#define HTTPRESPONSE_HPP_

#include "TemplateParser.hpp"
#include "NodeppError.hpp"
#include "HTTPRequest.hpp"
#include <unordered_map>
#include <iostream>
#include <stdio.h>
#include <string>

namespace NPPcore {

class HTTPResponse {
private:
	int code;
	std::string body;
	HTTPRequest& request;
	std::unordered_map<std::string,std::string> options;
public:
	const static std::unordered_map<int,std::string> codes;
	HTTPResponse(HTTPRequest& request, int code);
	void send(FILE* socket_pointer);
	void setCode(int code);
	void setOption(const std::string& name, const std::string& value);
	void setBody(const std::string& body);
	void appendBody(const std::string& part);
	void bindTemplate(const std::string& tplname);
	void bindTemplate(const std::string& tplname, const std::unordered_map<std::string, std::string>& variables);
	int getCode() const;
	std::string getOption(const std::string& name) const;
	std::string getBody() const;
	friend std::ostream& operator<<(std::ostream &strm, const HTTPResponse& resp);
	virtual ~HTTPResponse();
};

std::ostream& operator<<(std::ostream &strm, const HTTPResponse& resp);

} /* namespace NPPcore */

#endif /* HTTPRESPONSE_HPP_ */
