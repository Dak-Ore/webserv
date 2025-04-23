#include "SessionManager.hpp"

// Default Constructor
SessionManager::SessionManager() {
    std::cout << "SessionManager default constructor called" << std::endl;
}

// Copy Constructor
SessionManager::SessionManager(const SessionManager& other) {
    std::cout << "SessionManager copy constructor called" << std::endl;
    *this = other;
}

// Copy Assignment Operator
SessionManager& SessionManager::operator=(const SessionManager& other) {
    std::cout << "SessionManager copy assignment operator called" << std::endl;
    if (this != &other) {
        // Copy attributes here
    }
    return *this;
}

// Destructor
SessionManager::~SessionManager() {
    std::cout << "SessionManager destructor called" << std::endl;
}
