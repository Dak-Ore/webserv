#include "ConfigParser.hpp"

// Default Constructor
ConfigParser::ConfigParser() {
    std::cout << "ConfigParser default constructor called" << std::endl;
}

// Copy Constructor
ConfigParser::ConfigParser(const ConfigParser& other) {
    std::cout << "ConfigParser copy constructor called" << std::endl;
    *this = other;
}

// Copy Assignment Operator
ConfigParser& ConfigParser::operator=(const ConfigParser& other) {
    std::cout << "ConfigParser copy assignment operator called" << std::endl;
    if (this != &other) {
        // Copy attributes here
    }
    return *this;
}

// Destructor
ConfigParser::~ConfigParser() {
    std::cout << "ConfigParser destructor called" << std::endl;
}
