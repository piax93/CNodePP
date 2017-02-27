#include "HTTPRequest.hpp"

namespace NPPcore {

void HTTPRequest::load(int socketfd){
	FILE* socket_pointer = fdopen(socketfd, "r");
	size_t byteread, len = MAX_OPTION_LEN;
	char* linebuffer = (char*) malloc(MAX_OPTION_LEN);

	// Method, query, version
	byteread = getline(&linebuffer, &len, socket_pointer);
	linebuffer[byteread-2] = '\0';
	std::vector<std::string> s = util::splitString(linebuffer, ' ');
	version = s[2];
	if(s[0] == "GET") requestType = GET;
	else if(s[0] == "POST") requestType = POST;
	else if(s[0] == "PUT") requestType = PUT;
	else if(s[0] == "HEAD") requestType = HEAD;
	else throw NodeppUnsupported("Unknow request method");
	if(s[1][0] != '/') throw NodeppUnsupported("Incorrect route");
	s = util::splitString(s[1], "?");
	if(s[0].length() <= 1) route = DEFAULT_ROUTE;
	else route = util::urlDecode(s[0].substr(1));
	if(s.size() > 1) urlEncodedToMap(s[1], urlparams);

	// Options
	char* colon;
	while(true){
		byteread = getline(&linebuffer, &len, socket_pointer);
		if(strcmp(linebuffer, "\r\n") == 0) break;
		linebuffer[byteread-2] = '\0';
		colon = strchr(linebuffer, ':');
		if(colon == NULL) continue;
		*colon = '\0';
		options[std::string(linebuffer)] = util::trim(std::string(colon+1));
	}

	// POST contents
	if(requestType == POST) {
		if(util::startsWith(options.find("Content-Type")->second, "application/x-www-form-urlencoded")){
			size_t postlen = std::stoi(options.find("Content-Length")->second);
			if(postlen > len-1) linebuffer = (char*) realloc(linebuffer, postlen+1);
			byteread = fread(linebuffer, 1, postlen, socket_pointer);
			linebuffer[byteread] = '\0';
			urlEncodedToMap(std::string(linebuffer), postparams);
		} else if(util::startsWith(options.find("Content-Type")->second, "multipart/form-data")){
			// TODO
		}
	}

	free(linebuffer);
}

bool HTTPRequest::isStatic() const {
	return util::startsWith(route, STATIC_KEYWORD);
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

std::string HTTPRequest::$GET(const std::string& key) const {
	std::unordered_map<std::string,std::string>::const_iterator it = urlparams.find(key);
	return it == urlparams.end() ? "" : it->second;
}

std::string HTTPRequest::$POST(const std::string& key) const {
	std::unordered_map<std::string,std::string>::const_iterator it = postparams.find(key);
	return it == postparams.end() ? "" : it->second;
}

std::string HTTPRequest::getOption(const std::string& name) const {
	std::unordered_map<std::string,std::string>::const_iterator it = options.find(name);
	if(it == options.end()) throw NPPcore::NodeppError("Option '" + name + "' not set");
	return it->second;
}

void urlEncodedToMap(const std::string& data, std::unordered_map<std::string, std::string>& map) {
	size_t pos;
	std::vector<std::string> s = util::splitString(data, "&");
	for(unsigned i = 0; i < s.size(); i++){
		pos = s[i].find('=');
		if(pos == 0) continue;
		if(pos != s[i].npos) map[util::urlDecode(s[i].substr(0, pos))] = util::urlDecode(s[i].substr(pos+1));
		else map[util::urlDecode(s[i].substr(0, pos))] = "";
	}
}

std::ostream& operator<<(std::ostream &strm, const HTTPRequest& req) {
	std::string m[4] = {"GET", "HEAD", "POST", "PUT"};
	strm << "Method -> " << m[req.requestType] << "\n";
	strm << "Client -> " << req.clientip << "\n";
	strm << "Route -> " << req.route << "\n";
	std::unordered_map<std::string,std::string>::const_iterator it;
	strm << "Get -> [";
	for(it = req.urlparams.begin(); it != req.urlparams.end(); it++) strm << it->first << "->" << it->second << " & ";
	strm << "]\n";
	strm << "Post -> [";
	for(it = req.postparams.begin(); it != req.postparams.end(); it++) strm << it->first << "->" << it->second << " & ";
	strm << "]\n";
	strm << "Options -> [";
	for(it = req.options.begin(); it != req.options.end(); it++) strm << it->first << "->" << it->second << " || ";
	strm << "]\n";
	return strm;
}


} /* namespace NPPcore */
