#ifndef AUTOMATEEXCEPTION_H
#define AUTOMATEEXCEPTION_H

#include <string>

class AutomateException {
    std::string info;
public:
    AutomateException(const std::string& message): info(message) {}
    std::string getInfo() const { return info; }
};

#endif // AUTOMATEEXCEPTION_H
