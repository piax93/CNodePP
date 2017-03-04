#ifndef HTTPREQUEST_HPP_
#define HTTPREQUEST_HPP_

#include "NodeppError.hpp"
#include <unordered_map>
#include <netinet/in.h>
#include "strutils.hpp"
#include "netutils.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <string>
#include <array>
#include <regex>

#define MAX_OPTION_LEN 8192
#define DEFAULT_ROUTE "index"
#define STATIC_KEYWORD "static/"
#define TEMPDIR_TEMPLATE "/tmp/cnodeppXXXXXX"

namespace NPPcore {

enum REQUEST_TYPE {
    GET, HEAD, POST, PUT
};

struct FormFile {
    std::string tmppath;
    std::string filename;
    std::string mime;
    size_t size;
};

class HTTPRequest {
private:
    REQUEST_TYPE requestType;
    std::string version;
    std::string route;
    std::string clientip;
    std::unordered_map<std::string, std::string> options;
    std::unordered_map<std::string, std::string> urlparams;
    std::unordered_map<std::string, std::string> postparams;
    std::unordered_map<std::string, struct FormFile> files;
    void parseMultipart(Socket socketfd, const std::string& boundary);
public:
    HTTPRequest(struct sockaddr_in client): requestType(GET), clientip(inet_ntoa(client.sin_addr)) {}
    void load(int socketfd);
    std::string getVersion() const;
    std::string getRoute() const;
    REQUEST_TYPE getType() const;
    bool isStatic() const;
    std::string getOption(const std::string& name) const;
    std::string $GET(const std::string& key) const;
    std::string $POST(const std::string& key) const;
    friend std::ostream& operator<<(std::ostream &strm, const HTTPRequest& req);
    virtual ~HTTPRequest() {}
};

void urlEncodedToMap(const std::string& data, std::unordered_map<std::string, std::string>& map);
std::ostream& operator<<(std::ostream &strm, const HTTPRequest& req);

} /* namespace NPPcore */

#endif /* HTTPREQUEST_HPP_ */
