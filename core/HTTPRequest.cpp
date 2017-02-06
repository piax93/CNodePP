#include "HTTPRequest.hpp"

namespace NPPcore {

void HTTPRequest::load(FILE* socket_pointer){
	size_t byteread, len = MAX_OPTION_LEN;
	char* linebuffer = new char[MAX_OPTION_LEN];

	// Method, query, version
	byteread = getline(&linebuffer, &len, socket_pointer);
	linebuffer[byteread-2] = '\0';
	std::vector<std::string> s = util::splitString(linebuffer, ' ');
	version = s[2];
	if(s[0] == "GET") requestType = GET;
	else if(s[0] == "POST") requestType = POST;
	else if(s[0] == "PUT") requestType = PUT;
	else if(s[0] == "HEAD") requestType = HEAD;
	else throw NodeppError("Unknow request method");
	s = util::splitString(s[1], "?");
	if(s[0].length() <= 1) route = DEFAULT_ROUTE;
	else route = util::urlDecode(s[0].substr(1));
	if(s.size() > 1) {
		s = util::splitString(s[1], "&");
		for(unsigned i = 0; i < s.size(); i++){
			size_t pos = s[i].find('=');
			if(pos == 0) continue;
			if(pos != s[i].npos) urlparams[util::urlDecode(s[i].substr(0, pos))] = util::urlDecode(s[i].substr(pos+1));
			else urlparams[util::urlDecode(s[i].substr(0, pos))] = "";
		}
	}

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

std::string HTTPRequest::getRoute() const {
	return route;
}

std::string HTTPRequest::getOption(const std::string& name) const {
	std::unordered_map<std::string,std::string>::const_iterator it = options.find(name);
	if(it == options.end()) throw NPPcore::NodeppError("Option '" + name + "' not set");
	return it->second;
}

std::ostream& operator<<(std::ostream &strm, const HTTPRequest& req) {
	std::string m[4] = {"GET", "HEAD", "POST", "PUT"};
	strm << "Method -> " << m[req.requestType] << "\n";
	strm << "Route -> " << req.route << "\n";
	std::unordered_map<std::string,std::string>::const_iterator it;
	strm << "Get -> [";
	for(it = req.urlparams.begin(); it != req.urlparams.end(); it++) strm << it->first << "->" << it->second << " & ";
	strm << "]\n";
	strm << "Options -> [";
	for(it = req.options.begin(); it != req.options.end(); it++) strm << it->first << "->" << it->second << " || ";
	strm << "]\n";
	return strm;
}


} /* namespace NPPcore */
