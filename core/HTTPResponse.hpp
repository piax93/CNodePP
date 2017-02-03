#ifndef HTTPRESPONSE_HPP_
#define HTTPRESPONSE_HPP_

#include "NodeppError.hpp"
#include <unordered_map>
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
	void setOption(std::string name, std::string value);
	int getCode() const;
	std::string getOption(const std::string& name) const;
	virtual ~HTTPResponse();
};

} /* namespace NPPcore */

#endif /* HTTPRESPONSE_HPP_ */
