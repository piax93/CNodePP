#include "HTTPResponse.hpp"

namespace NPPcore {

const std::unordered_map<int,std::string> HTTPResponse::codes = {
        {200, "OK"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {500, "Internal Server Error"},
        {501, "Not Implemented"}
};

HTTPResponse::HTTPResponse(HTTPRequest& _request, int _code): code(_code), request(_request),
        bindCount(0), body(""), staticFilename("") {
    options["Content-Type"] = "text/html; charset=UTF-8";
    options["Server"] = "NodePP";
}

void HTTPResponse::send(int socketfd){
    bool isStaticFile = staticFilename.length() > 0;
    char responseLine[MAX_RESPONSE_LINE];
    sprintf(responseLine, "%s %d %s\r\n", request.getVersion().c_str(), code, codes.find(code)->second.c_str());
    util::sendn(socketfd, responseLine, strlen(responseLine));
    options["Content-Length"] = std::to_string(isStaticFile ? util::getFileSize(staticFilename) : body.length());
    if(isStaticFile) options["Content-Type"] = util::getMimeType(staticFilename);
    for(auto it = options.begin(); it != options.end(); it++) {
        sprintf(responseLine, "%s: %s\r\n", it->first.c_str(), it->second.c_str());
        util::sendn(socketfd, responseLine, strlen(responseLine));
    }
    util::sendn(socketfd, "\r\n", 2);
    if(isStaticFile) util::sendFile(socketfd, staticFilename);
    else util::sendn(socketfd, body.c_str(), body.size());
}

void HTTPResponse::setStaticFile(const std::string& fileroute){
    std::string staticdir = Configuration::getInstance().getValue("static_dir");
    std::string filename = fileroute.substr(strlen(STATIC_KEYWORD));
    if(filename.length() == 0) {
        body = "AHAH! Sorry, I won't give you any directory listing.";
        return;
    }
    staticFilename = staticdir + "/" + filename;
    if(!util::startsWith(realpath(staticFilename.c_str(), NULL), realpath(staticdir.c_str(), NULL))
            || !util::isRegularFile(staticFilename)) {
        staticFilename = "";
        throw NodeppNotFound("Directory traversal tentative or invalid path");
    }
}

void HTTPResponse::bindTemplate(const std::string& tplfilename,
        const std::unordered_map<std::string, std::string>& variables, bool statictpl) {
    if(bindCount > MAX_RECURSION) {
        std::cerr << "BindTemplate recursion limit exceeded" << std::endl;
        return;
    }
    Configuration& conf = Configuration::getInstance();
    ModuleLoader& ml = ModuleLoader::getInstance();
    std::string templ = util::readFileToString(conf.getValue("template_dir") + "/" + tplfilename);
    body = templ;
    if(statictpl) return;
    std::regex pattern("\\{[\\{\\%]\\s*([a-zA-Z0-9_]+)\\s*[\\}\\%]\\}");
    std::regex_iterator<std::string::iterator> it(templ.begin(), templ.end(), pattern);
    std::regex_iterator<std::string::iterator> itend;
    HTTPResponse dummy(request, 200);
    int offset = 0;
    std::string sub;
    bool sub_set;
    while(it != itend){
        sub_set = false;
        if(it->str()[1] == '%' && ml.hasModule(it->str(1))){
            dummy.body = "";
            dummy.bindCount = bindCount + 1;
            dummy.options = options;
            ml.getPage(it->str(1), request, dummy);
            sub = dummy.getBody();
            sub_set = true;
        } else {
            auto varvalue = variables.find(it->str(1));
            if(varvalue != variables.end()) {
                sub = varvalue->second;
                sub_set = true;
            }
        }
        if(sub_set){
            body.replace(it->position() + offset, it->length(), sub);
            offset += sub.length() - it->length();
        }
        it++;
    }
}

void HTTPResponse::bindTemplate(const std::string& tplname, bool statictpl) {
    bindTemplate(tplname, std::unordered_map<std::string,std::string>(), statictpl);
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
