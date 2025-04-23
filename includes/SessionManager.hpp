#ifndef SESSIONMANAGER_HPP
# define SESSIONMANAGER_HPP

# include <iostream>

class SessionManager
{
public:
    // Default Constructor
    SessionManager();
    
    // Copy Constructor
    SessionManager(const SessionManager& other);
    
    // Copy Assignment Operator
    SessionManager& operator=(const SessionManager& other);
    
    // Destructor
    ~SessionManager();
};

#endif
