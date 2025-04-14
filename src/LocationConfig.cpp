#include "LocationConfig.hpp"

// Default Constructor
LocationConfig::LocationConfig() {
    std::cout << "LocationConfig default constructor called" << std::endl;
}

// Copy Constructor
LocationConfig::LocationConfig(const LocationConfig& other) {
    std::cout << "LocationConfig copy constructor called" << std::endl;
    *this = other;
}

// Copy Assignment Operator
LocationConfig& LocationConfig::operator=(const LocationConfig& other) {
    std::cout << "LocationConfig copy assignment operator called" << std::endl;
    if (this != &other) {
        // Copy attributes here
    }
    return *this;
}

// Destructor
LocationConfig::~LocationConfig() {
    std::cout << "LocationConfig destructor called" << std::endl;
}
