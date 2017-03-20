#include "strutils.hpp"

namespace util {

std::vector<std::string> splitString(const std::string& src, const std::string& delimiter){
    std::vector<std::string> res;
    std::string tmp(src);
    size_t pos = 0;
    while(true){
        pos = tmp.find(delimiter);
        if(pos == std::string::npos) {
            res.push_back(tmp);
            break;
        }
        res.push_back(tmp.substr(0, pos));
        tmp = tmp.substr(pos+delimiter.length());
    }
    return res;
}


std::vector<std::string> splitString(char* src, char delimiter){
    std::vector<std::string> res;
    char* pos;
    char* tmp = src;
    while(true){
        pos = strchr(tmp, delimiter);
        if(pos == NULL) {
            res.push_back(std::string(tmp));
            break;
        }
        *pos = '\0';
        res.push_back(std::string(tmp));
        *pos = delimiter;
        tmp = pos + 1;
    }
    return res;
}


std::string charToHex(unsigned char c) {
    short i = c;
    std::stringstream s;
    s << "%" << std::setw(2) << std::setfill('0') << std::hex << i;
    return s.str();
}

unsigned char hexToChar(const std::string &str) {
    short c = 0;
    if(!str.empty()) {
        std::istringstream in(str);
        in >> std::hex >> c;
    }
    return (unsigned char) c;
}

std::string urlEncode(const std::string& toEncode) {
    std::ostringstream out;
    for(std::string::size_type i=0; i < toEncode.length(); ++i) {
        short t = toEncode.at(i);
        if(
            t == 45 ||                    // hyphen
            (t >= 48 && t <= 57) ||       // 0-9
            (t >= 65 && t <= 90) ||       // A-Z
            t == 95 ||                    // underscore
            (t >= 97 && t <= 122) ||      // a-z
            t == 126                      // tilde
        ) {
            out << toEncode.at(i);
        } else {
            out << charToHex(toEncode.at(i));
        }
    }
    return out.str();
}

std::string urlDecode(const std::string &toDecode) {
    std::ostringstream out;
    for(std::string::size_type i=0; i < toDecode.length(); ++i) {
        if(toDecode.at(i) == '%') {
            std::string str(toDecode.substr(i+1, 2));
            out << hexToChar(str);
            i += 2;
        } else {
            out << toDecode.at(i);
        }
    }
    return out.str();
}


bool endsWith(const std::string& value, const std::string& ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool startsWith(const std::string& value, const std::string& prefix) {
    if (prefix.size() > value.size()) return false;
    return std::equal(prefix.begin(), prefix.end(), value.begin());
}

bool endsWith(const char* value, const char* ending) {
    if(value == NULL || ending == NULL) return false;
    size_t lenv = strlen(value);
    size_t lene = strlen(ending);
    if (lene > lenv) return false;
    for(size_t i = 1; i <= lene; i++) {
        if(value[lenv-i] != ending[lene-i]) return false;
    }
    return true;
}

bool startsWith(const char* value, const char* prefix) {
    if(value == NULL || prefix == NULL) return false;
    size_t lenv = strlen(value);
    size_t lenp = strlen(prefix);
    if (lenp > lenv) return false;
    for(size_t i = 0; i < lenp; i++) {
        if(value[i] != prefix[i]) return false;
    }
    return true;
}


std::string removeExtension(const char* filename){
    const char* dot = strrchr(filename, '.');
    return std::string(filename, dot-filename);
}

std::string removeExtension(const std::string& filename){
    size_t pos = filename.find_last_of('.');
    return filename.substr(0, pos);
}


std::string trim(const std::string& s) {
    int start = -1, end = s.length();
    while(start++ < end-1) if(s[start] != ' ' && s[start] != '\t' && s[start] != '\n' && s[start] != '\r') break;
    while(end-- > start) if(s[end] != ' ' && s[end] != '\t' && s[end] != '\n' && s[end] != '\r') break;
    return start <= end ? s.substr(start, end - start + 1) : "";
}

std::string unescape(const std::string& s) {
    char c;
    std::string res;
    auto it = s.begin();
    while (it != s.end()) {
        c = *it++;
        if (c == '\\' && it != s.end()) {
            switch (*it++) {
                case '\\': c = '\\'; break;
                case 'n' : c = '\n'; break;
                case 't' : c = '\t'; break;
                case '"' : c = '\"'; break;
                case '\'': c = '\''; break;
                case 'r' : c = '\r'; break;
                default: continue;
            }
        }
        res += c;
    }
    return res;
}

std::string nl2br(const std::string& s) {
    return std::regex_replace(s, std::regex("\n"), "<br>");
}

std::string readFileToString(const std::string& filename){
    try {
        std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
        std::ifstream::pos_type fileSize = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        std::vector<char> bytes(fileSize);
        ifs.read(&bytes[0], fileSize);
        return std::string(&bytes[0], fileSize);
    } catch (std::exception& e) {
        throw NPPcore::NodeppError("Cannot read file " + filename);
    }
}


}  /* namespace util */
