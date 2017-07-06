#include "HTTPRequest.hpp"
#include "Configuration.hpp"
#include "NodeppError.hpp"
#include "strutils.hpp"
#include <iostream>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <array>
#include <regex>

namespace NPPcore {

void HTTPRequest::load(int socketfd){
    ssize_t byteread;
    std::vector<char> linebuffer(MAX_OPTION_LEN + 1);
    char* linebufferpointer = linebuffer.data();

    // Method, query, version
    byteread = util::netgetline(socketfd, linebufferpointer, MAX_OPTION_LEN);
    linebuffer[byteread-2] = '\0';
    std::vector<std::string> s = util::splitString(linebufferpointer, ' ');
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
    while(true) {
        byteread = util::netgetline(socketfd, linebufferpointer, MAX_OPTION_LEN);
        if(strcmp(linebufferpointer, "\r\n") == 0) break;
        options.insert(unpackOption(linebufferpointer, byteread));
    }

    // POST contents
    if(requestType == POST) {
        size_t postlen = std::stoi(options.find("Content-Length")->second);
        Configuration& conf = Configuration::getInstance();
        if(util::startsWith(options.find("Content-Type")->second, "application/x-www-form-urlencoded")){
            if(postlen > MBYTE(conf.getValueToInt("max_post_size"))) throw NodeppError("POST too large");
            if(postlen > MAX_OPTION_LEN - 1) linebuffer.resize(postlen + 1);
            byteread = util::recvn(socketfd, linebuffer.data(), postlen);
            if(byteread < (ssize_t)postlen) throw NodeppError("Badly formatted POST request");
            linebuffer[byteread] = '\0';
            urlEncodedToMap(std::string(linebuffer.data()), postparams);
        } else if(util::startsWith(options.find("Content-Type")->second, "multipart/form-data")){
            if(postlen > MBYTE(conf.getValueToInt("max_upload_size"))) throw NodeppError("Upload too large");
            std::smatch boundary;
            std::regex bmatcher("multipart/form-data\\s*;\\s*boundary=(.*)$");
            if(std::regex_match(options.find("Content-Type")->second, boundary, bmatcher)) {
                parseMultipart(socketfd, boundary.str(1), postlen);
            }
        }
    }

}

void HTTPRequest::parseMultipart(Socket socketfd, const std::string& boundary, ssize_t clen){
    std::string bound = "--" + boundary;
    char* tempdirtemplate = strdup(TEMPDIR_TEMPLATE);
    std::string tempdir(mkdtemp(tempdirtemplate));
    std::vector<char> buffer(MP_BUFFER_LEN + 1);
    char* buffprt = buffer.data();
    std::pair<std::string, std::string> opt;
    size_t byteread;
    std::regex namematcher(";\\s*name=\"([^\"]*)\"");
    std::regex filenamematcher(";\\s*filename=\"([^\"]*)\"");
    std::smatch namematch;
    std::smatch filenamematch;
    // First boundary
    clen -= util::netgetline(socketfd, buffprt, MP_BUFFER_LEN);
    while(clen > 2){
        // Read option
        byteread = util::netgetline(socketfd, buffprt, MP_BUFFER_LEN);
        clen -= byteread;
        opt = unpackOption(buffprt, byteread);
        std::regex_search(opt.second, namematch, namematcher);
        if(std::regex_search(opt.second, filenamematch, filenamematcher)){
            // File
            // Content Type
            byteread = util::netgetline(socketfd, buffprt, MP_BUFFER_LEN);
            clen -= byteread;
            std::pair<std::string, std::string> ctype = unpackOption(buffprt, byteread);
            // Create file
            char* tempfiletemplate = strdup((tempdir + TEMPFILE_TEMPLATE).c_str());
            int targetfd = mkstemp(tempfiletemplate);
            if(targetfd < 0) throw NodeppError("Cannot open temp file");
            struct FormFile ff(std::string(tempfiletemplate), filenamematch.str(1), ctype.second);
            // Newline separator
            clen -= util::netgetline(socketfd, buffprt, MP_BUFFER_LEN);
            // Read the madness
            // Not too clever implementation (rewrite proper parsing in the future)
            ssize_t chunksize = 0, segendsize = 0;
            char segend[2];
            while(true){
                chunksize = util::netgetline(socketfd, buffprt, MP_BUFFER_LEN);
                if(chunksize <= 0 || util::startsWith(buffprt, bound.c_str())) break;
                else util::sendn(targetfd, segend, segendsize, true);
                segendsize = chunksize < 2 ? chunksize : 2;
                clen -= chunksize;
                ff.size += chunksize;
                util::sendn(targetfd, buffprt, chunksize - segendsize, true);
                for(int i = 0; i < segendsize; i++) segend[i] = buffprt[chunksize - segendsize + i];
            }
            clen -= chunksize;
            close(targetfd);
            free(tempfiletemplate);
            files[util::unescape(namematch.str(1))] = ff;
        } else {
            // POST data
            std::string value("");
            // Newline separator
            clen -= util::netgetline(socketfd, buffprt, MP_BUFFER_LEN);
            // Content Value
            while(true){
                clen -= util::netgetline(socketfd, buffprt, MP_BUFFER_LEN);
                if(util::startsWith(buffprt, bound.c_str())) break;
                value += std::string(buffprt);
            }
            postparams[util::unescape(namematch.str(1))] = value.substr(0, value.length()-2);
        }
    }
    free(tempdirtemplate);
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

struct FormFile HTTPRequest::$FILE(const std::string& key) const {
    std::unordered_map<std::string,struct FormFile>::const_iterator it = files.find(key);
    return it == files.end() ? FormFile() : it->second;
}

std::string HTTPRequest::getOption(const std::string& name) const {
    std::unordered_map<std::string,std::string>::const_iterator it = options.find(name);
    if(it == options.end()) throw NPPcore::NodeppError("Option '" + name + "' not set");
    return it->second;
}

HTTPRequest::~HTTPRequest() {
    for(auto it = files.begin(); it != files.end(); it++) remove(it->second.tmppath.c_str());
    if(files.size() > 0) {
        char* tmp = strdup(files.begin()->second.tmppath.c_str());
        remove(dirname(tmp));
        free(tmp);
    }
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

std::pair<std::string, std::string> unpackOption(char* linebuffer, size_t byteread){
    char* colon;
    linebuffer[byteread - 2] = '\0';
    colon = strchr(linebuffer, ':');
    if(colon == NULL) return std::pair<std::string, std::string>("_", "_");
    *colon = '\0';
    return std::pair<std::string, std::string>(std::string(linebuffer), util::trim(std::string(colon+1)));
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
