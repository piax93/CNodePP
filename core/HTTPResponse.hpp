#ifndef HTTPRESPONSE_HPP_
#define HTTPRESPONSE_HPP_

#include "NodeppError.hpp"
#include <unordered_map>
#include <iostream>
#include <stdio.h>
#include <string>

namespace NPPcore {

class HTTPResponse {
private:
	int code;
	std::string version;
	std::unordered_map<std::string,std::string> options;
	std::string body;
public:
	const static std::unordered_map<int,std::string> codes;
	HTTPResponse(std::string version, int code);
	void send(FILE* socket_pointer);
	void setCode(int code);
	void setOption(const std::string& name, const std::string& value);
	void setBody(const std::string& body);
	int getCode() const;
	std::string getOption(const std::string& name) const;
	friend std::ostream& operator<<(std::ostream &strm, const HTTPResponse& resp);
	virtual ~HTTPResponse();
};

std::ostream& operator<<(std::ostream &strm, const HTTPResponse& resp);

} /* namespace NPPcore */

#endif /* HTTPRESPONSE_HPP_ */
