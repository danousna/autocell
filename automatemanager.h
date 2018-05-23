#ifndef AUTOMATEMANAGER_H
#define AUTOMATEMANAGER_H

#include "automate.h"

class AutomateManager {
    Automate* automates[256];
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
    const Automate& getAutomate(unsigned short int num);
    const Automate& getAutomate(const std::string& num);
    static AutomateManager& getAutomateManager();
    static void libererAutomateManager();
};

#endif // AUTOMATEMANAGER_H
