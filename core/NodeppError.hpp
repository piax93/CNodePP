#ifndef NODEPPERROR_HPP_
#define NODEPPERROR_HPP_

#include <exception>
#include <iostream>
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

class NodeppNotFound: public std::exception {
private:
    std::string message_;
public:
    NodeppNotFound(const std::string& message) : message_(message) {}
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};

/**
 * Terminate the program displaying an error message
 * @param message Message to display
 * @param status Exit status
 */
inline void die(const std::string& message, int status=-1){
    std::cerr << message << std::endl;
    std::exit(status);
}

} /* namespace NPPcore */

#endif /* NODEPPERROR_HPP_ */
