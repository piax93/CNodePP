#include "HTTPResponse.hpp"

namespace NPPcore {

const std::unordered_map<int,std::string> HTTPResponse::codes =
	{{200, "OK"}, {403, "Forbidden"}, {404, "Not Found"}, {500, "Internal Server Error"}};

HTTPResponse::HTTPResponse(HTTPRequest& _request, int code): request(_request) {
	this->code = code;
	bindCount = 0;
	options["Content-Type"] = "text/html";
	options["Server"] = "NodePP";
	body = "";
}

void HTTPResponse::send(FILE* socket_pointer){
	fprintf(socket_pointer, "%s %d %s\r\n", request.getVersion().c_str(), code, codes.find(code)->second.c_str());
	options["Content-Length"] = std::to_string(body.length());
	for(auto it = options.begin(); it != options.end(); it++)
		fprintf(socket_pointer, "%s:%s\r\n", it->first.c_str(), it->second.c_str());
	fputs("\r\n", socket_pointer);
	fputs(body.c_str(), socket_pointer);
	fflush(socket_pointer);
}

void HTTPResponse::bindTemplate(const std::string& tplfilename, const std::unordered_map<std::string, std::string>& variables) {
	if(bindCount > MAX_RECURSION) {
		std::cerr << "BindTemplate recursion limit exceeded" << std::endl;
		return;
	}
	Configuration& conf = Configuration::getInstance();
	ModuleLoader& ml = ModuleLoader::getInstance();
	std::string templ = util::readFileToString(conf.getValue("template_dir") + "/" + tplfilename);
	body = templ;
	std::regex pattern("\\{[\\{\\%]\\s*([a-zA-Z0-9_]+)\\s*[\\}\\%]\\}");
	std::regex_iterator<std::string::iterator> it(templ.begin(), templ.end(), pattern);
	std::regex_iterator<std::string::iterator> itend;
	HTTPResponse dummy(request, 200);
	int offset = 0;
	std::string sub;
	while(it != itend){
		sub = "";
		if(it->str()[1] == '%' && ml.hasModule(it->str(1))){
			dummy.body = "";
			dummy.bindCount = bindCount + 1;
			dummy.options = options;
			ml.getPage(it->str(1), request, dummy);
			sub = dummy.getBody();
		} else {
			auto varvalue = variables.find(it->str(1));
			if(varvalue != variables.end()) sub = varvalue->second;
		}
		if(sub.length() > 0){
			body.replace(it->position() + offset, it->length(), sub);
			offset += sub.length() - it->length();
		}
		it++;
	}
}

void HTTPResponse::bindTemplate(const std::string& tplname) {
	bindTemplate(tplname, std::unordered_map<std::string,std::string>());
}

void HTTPResponse::setCode(int code) {
	this->code = code;
}

void HTTPResponse::setOption(const std::string& name, const std::string& value) {
	options[name] = value;
}

int HTTPResponse::getCode() const {
	return code;
}

void HTTPResponse::setBody(const std::string& body) {
	this->body = body;
}

void HTTPResponse::appendBody(const std::string& part) {
	body += part;
}

std::string HTTPResponse::getBody() const {
	return body;
}

std::string HTTPResponse::getOption(const std::string& name) const {
	std::unordered_map<std::string,std::string>::const_iterator it = options.find(name);
	if(it == options.end()) throw NPPcore::NodeppError("Option '" + name + "' not set");
	return it->second;
}

std::ostream& operator<<(std::ostream &strm, const HTTPResponse& resp){
	strm << resp.request.getVersion() << ' ' << resp.code << ' ' << resp.codes.find(resp.code)->second << std::endl;
	std::unordered_map<std::string,std::string>::const_iterator it;
	strm << "Options -> [";
	for(it = resp.options.begin(); it != resp.options.end(); it++) strm << it->first << "->" << it->second << " || ";
	strm << "]\n\n";
	return strm << resp.body;
}

HTTPResponse::~HTTPResponse() { }

} /* namespace NPPcore */
