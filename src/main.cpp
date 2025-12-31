#include "SimpleMenu.hpp"
#include "ServiceManager.hpp"
#include "RepairRequestFactory.hpp"

int main() {
    // Get ServiceManager singleton instance
    ServiceManager& manager = ServiceManager::getInstance();
    
    // Create RepairRequestFactory
    RepairRequestFactory requestFactory(1);
    
    // Create and start the simple menu
    SimpleMenu menu(manager, requestFactory);
    menu.start();
    
    return 0;
}