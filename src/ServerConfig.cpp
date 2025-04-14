#include "ServerConfig.hpp"

// Default Constructor
ServerConfig::ServerConfig() {
    std::cout << "ServerConfig default constructor called" << std::endl;
}

// Copy Constructor
ServerConfig::ServerConfig(const ServerConfig& other) {
    std::cout << "ServerConfig copy constructor called" << std::endl;
    *this = other;
}

// Copy Assignment Operator
ServerConfig& ServerConfig::operator=(const ServerConfig& other) {
    std::cout << "ServerConfig copy assignment operator called" << std::endl;
    if (this != &other) {
        // Copy attributes here
    }
    return *this;
}

// Destructor
ServerConfig::~ServerConfig() {
    std::cout << "ServerConfig destructor called" << std::endl;
}
