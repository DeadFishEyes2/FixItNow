// ServiceManagerTUI.hpp
#pragma once

#include "ServiceManager.hpp"
#include <ncurses.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <string>
#include <vector>

enum class MenuState {
    Main,
    EmployeeManagement,
    RequestManagement,
    ViewStatistics,
    AddTechnician,
    AddReceptionist,
    AddSupervisor,
    RemoveEmployee,
    ListEmployees,
    AddRequest,
    ViewRequest,
    ListRequests,
    FilterRequests
};

class ServiceManagerTUI {
private:
    ServiceManager& manager;
    RepairRequestFactory& requestFactory;
    
    std::atomic<bool> running{true};
    std::mutex managerMutex;
    std::thread tickThread;
    
    MenuState currentMenu;
    int selectedItem;
    int scrollOffset;
    
    // UI dimensions
    int maxY, maxX;
    
public:
    ServiceManagerTUI(ServiceManager& mgr, RepairRequestFactory& reqFactory);
    ~ServiceManagerTUI();
    
    void start();
    
private:
    // Core loops
    void tickLoop();
    void mainUILoop();
    
    // Drawing methods
    void drawUI();
    void drawHeader();
    void drawFooter();
    void drawMainMenu();
    void drawEmployeeMenu();
    void drawRequestMenu();
    void drawStatistics();
    
    // Input handling
    void handleInput();
    void handleSelection();
    
    // Menu actions - Employees
    void showAddTechnicianForm();
    void showAddReceptionistForm();
    void showAddSupervisorForm();
    void showRemoveEmployeeDialog();
    void showListEmployees();
    
    // Menu actions - Requests
    void showAddRequestForm();
    void showViewRequestDialog();
    void showListRequests();
    void showFilterRequestsDialog();
    
    // Helper methods
    void showMessage(const std::string& message);
    std::string getInput(const std::string& prompt, int row, int col);
    int getIntInput(const std::string& prompt, int row, int col);
    void waitForKey();
    void clearContent();
    std::string statusToString(Status status);
    std::string employeeTypeToString(EmployeeType type);
    
    // Address input helper
    Address getAddressInput(int startRow, int col);
    
    // Appliance input helper
    std::unique_ptr<Appliance> getApplianceInput(int startRow, int col);
};