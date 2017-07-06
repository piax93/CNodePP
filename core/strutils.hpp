#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <vector>
#include <string>


namespace util {

/**
 * Split string into tokens
 * @param src String to split
 * @param delimiter Delimiter string
 * @return Vector of tokens
 */
std::vector<std::string> splitString(const std::string& src, const std::string& delimiter);

/**
 * Split string into tokens
 * @param src Char array to split
 * @param delimiter Delimiter character
 * @return Array of tokens
 */
std::vector<std::string> splitString(char* src, char delimiter);

/**
 * Check if string ends with a certain suffix
 * @param value String to check
 * @param ending Ending
 */
bool endsWith(const std::string& value, const std::string& ending);

/**
 * Check if string starts with prefix
 * @param value String to check
 * @param prefix Prefix
 */
bool startsWith(const std::string& value, const std::string& prefix);

/**
 * Check if string ends with a certain suffix
 * @param value Char array to check
 * @param ending Ending
 */
bool endsWith(const char* value, const char* ending);

/**
 * Check if string starts with prefix
 * @param value Char array to check
 * @param prefix Prefix
 */
bool startsWith(const char* value, const char* prefix);

/**
 * Remove extension from filename
 */
std::string removeExtension(const char* filename);

/**
 * Remove extension from filename
 */
std::string removeExtension(const std::string& filename);

/**
 * Encode string using URL notations
 */
std::string urlEncode(const std::string& toEncode);

/**
 * Decode URL-Encoded string
 */
std::string urlDecode(const std::string &toDecode);

/**
 * Store a complete text file in a string
 * @param filename File path
 * @return String containing file content
 */
std::string readFileToString(const std::string& filename);

/**
 * Trim both ends of a string
 */
std::string trim(const std::string& s);

/**
 * Transform explicit escape symbols into ASCII values
 */
std::string unescape(const std::string& s);

/**
 * Convert newline chatacters to <br> tags
 */
std::string nl2br(const std::string& s);

}  /* namespace util */

#endif /* UTILITIES_HPP_ */
