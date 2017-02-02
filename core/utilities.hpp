#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <string.h>
#include <vector>
#include <string>

namespace util {

std::vector<std::string> splitString(const std::string& src, const std::string& delimiter);
std::vector<std::string> splitString(char* src, char delimiter);
bool ends_with(std::string const & value, std::string const & ending);

}  /* namespace util */

#endif /* UTILITIES_HPP_ */
