#include "HTTPRequest.hpp"

namespace NPPcore {

void HTTPRequest::load(FILE* socket_pointer){
	size_t byteread, len = MAX_OPTION_LEN;
	char* linebuffer = new char[MAX_OPTION_LEN];

	// Method, query, version
	byteread = getline(&linebuffer, &len, socket_pointer);
	linebuffer[byteread-2] = '\0';
	std::vector<std::string> s = util::splitString(linebuffer, ' ');
	query = s[1]; version = s[2];
	if(s[0] == "GET") requestType = GET;
	else if(s[0] == "POST") requestType = POST;
	else if(s[0] == "PUT") requestType = PUT;
	else if(s[0] == "HEAD") requestType = HEAD;
	else throw NodeppError("Unknow request method");

	// Options
	char* colon;
	while(true){
		byteread = getline(&linebuffer, &len, socket_pointer);
		if(byteread <= 2) break;
		linebuffer[byteread-2] = '\0';
		colon = strchr(linebuffer, ':');
		if(colon == NULL) continue;
		*colon = '\0';
		options[std::string(linebuffer)] = std::string(colon+1);
	}

}

std::string HTTPRequest::getVersion() const {
	return version;
}

REQUEST_TYPE HTTPRequest::getType() const {
	return requestType;
}

std::string HTTPRequest::getOption(const std::string& name) const {
	std::unordered_map<std::string,std::string>::const_iterator it = options.find(name);
	if(it == options.end()) throw NPPcore::NodeppError("Option '" + name + "' not set");
	return it->second;
}

std::ostream& operator<<(std::ostream &strm, const HTTPRequest& req) {
	std::string m[4] = {"GET", "HEAD", "POST", "PUT"};
	strm << "Method -> " << m[req.requestType] << "\n";
	strm << "Query -> " << req.query << "\n";
	std::unordered_map<std::string,std::string>::const_iterator it;
	strm << "Options -> [";
	for(it = req.options.begin(); it != req.options.end(); it++) strm << it->first << "->" << it->second << " || ";
	strm << "]\n";
	return strm;
}


} /* namespace NPPcore */
