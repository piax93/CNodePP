#include "utilities.hpp"

namespace util {

/**
 * Split string into tokens
 * @param src String to split
 * @param delimiter Delimiter string
 * @return Array of tokens
 */
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

/**
 * Split string into tokens
 * @param src String to split
 * @param delimiter Delimiter character
 * @return Array of tokens
 */
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

/**
 * Encode string using URL notations
 */
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

/**
 * Decode URL-Encoded string
 */
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

/**
 * Check if string end with a certain suffix
 * @param value String to check
 * @param ending Ending
 */
bool endsWith(const std::string& value, const std::string& ending) {
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

/**
 * Check if string end with a certain suffix
 * @param value String to check
 * @param ending Ending
 */
bool endsWith(const char* value, const std::string& ending) {
	size_t lenv = strlen(value);
	size_t lene = ending.length();
	if (lene > lenv) return false;
	for(size_t i = 1; i <= lene; i++) {
		if(value[lenv-i] != ending[lene-i]) return false;
	}
	return true;
}

/**
 * Remove extension from filename
 */
std::string removeExtension(const char* filename){
	const char* dot = strrchr(filename, '.');
	return std::string(filename, dot-filename);
}

/**
 * Remove extension from filename
 */
std::string removeExtension(const std::string& filename){
	size_t pos = filename.find_last_of('.');
	return filename.substr(0, pos);
}

/**
 * Trim start of a string
 */
std::string ltrim(const std::string& s) {
    std::string res(s);
	res.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return res;
}

/**
 * Trim end of a string
 */
std::string rtrim(const std::string& s) {
    std::string res(s);
	res.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return res;
}

/**
 * Trim both ends of a string
 */
std::string trim(const std::string& s) {
    return ltrim(rtrim(s));
}

/**
 * Store a complete text file in a string
 * @param filename File path
 * @return String containing file content
 */
std::string readFileToString(const std::string& filename){
	std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	std::ifstream::pos_type fileSize = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::vector<char> bytes(fileSize);
	ifs.read(&bytes[0], fileSize);
	return std::string(&bytes[0], fileSize);
}

/**
 * Terminate the program displaying an error message
 * @param message Message to display
 * @param status Exit status
 */
void die(const std::string& message, int status){
	std::cerr << message << std::endl;
	std::exit(status);
}

}  /* namespace util */
