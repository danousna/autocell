#ifndef AUTOMATEMANAGER_H
#define AUTOMATEMANAGER_H

#include "automate1d.h"

class AutomateManager {
    AutomateElementaire* automates[256];
    AutomateManager();
    ~AutomateManager();
    AutomateManager(const AutomateManager& a);
    AutomateManager& operator=(const AutomateManager& a);
    struct Handler {
        AutomateManager* instance;
        Handler():instance(nullptr) {}
        ~Handler() { delete instance;  }
    };
    static Handler handler;
public:
    const Automate& getAutomate(unsigned int d, unsigned int e, unsigned int num);
    const Automate& getAutomate(unsigned int d, unsigned int e, const std::string& num);
    static AutomateManager& getAutomateManager();
    static void libererAutomateManager();
};

#endif // AUTOMATEMANAGER_H
