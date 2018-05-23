#include "automatemanager.h"

AutomateManager::AutomateManager() {
    for (unsigned int i = 0; i < 256; i++) {
        automates[i] = nullptr;
    }
}

AutomateManager::~AutomateManager() {
    for (unsigned int i = 0; i < 256; i++) {
        delete automates[i];
    }
}

AutomateManager::Handler AutomateManager::handler = Handler();

AutomateManager& AutomateManager::getAutomateManager() {
    if (!handler.instance) {
        handler.instance = new AutomateManager;
    }
    return *handler.instance;
}

void AutomateManager::libererAutomateManager() {
    delete handler.instance;
    handler.instance = nullptr;
}

const Automate& AutomateManager::getAutomate(unsigned short int num) {
    if (!automates[num]) {
        automates[num] = new Automate(num);
    }
    return *automates[num];
}
const Automate& AutomateManager::getAutomate(const std::string& numBit) {
    return getAutomate(NumBitToNum(numBit));
}
