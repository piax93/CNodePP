#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <string.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>


namespace util {

std::vector<std::string> splitString(const std::string& src, const std::string& delimiter);
std::vector<std::string> splitString(char* src, char delimiter);
bool endsWith(const std::string& value, const std::string& ending);
bool endsWith(const char* value, const std::string& ending);
std::string removeExtension(const char* filename);
std::string removeExtension(const std::string& filename);
void die(const std::string& message, int status=-1);
std::string urlEncode(const std::string& toEncode);
std::string urlDecode(const std::string &toDecode);

}  /* namespace util */

#endif /* UTILITIES_HPP_ */
