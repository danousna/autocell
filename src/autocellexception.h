#ifndef AUTOCELLEXCEPTION_H
#define AUTOCELLEXCEPTION_H

#include <string>

class AutoCellException {
    std::string info;
public:
    AutoCellException(const std::string& message): info(message) {}
    std::string getInfo() const { return info; }
};

#endif // AUTOCELLEXCEPTION_H
