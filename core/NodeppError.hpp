#ifndef NODEPPERROR_HPP_
#define NODEPPERROR_HPP_

#include <exception>
#include <string>

namespace NPPcore {

class NodeppError: public std::exception {
private:
	std::string message_;
public:
	NodeppError(const std::string& message) : message_(message) {}
	virtual const char* what() const throw() {
		return message_.c_str();
	}
};

class NodeppUnsupported: public std::exception {
private:
	std::string message_;
public:
	NodeppUnsupported(const std::string& message) : message_(message) {}
	virtual const char* what() const throw() {
		return message_.c_str();
	}
};

} /* namespace NPPcore */

#endif /* NODEPPERROR_HPP_ */
