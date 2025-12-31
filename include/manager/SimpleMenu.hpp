#pragma once

#include <iostream>
#include <string>
#include <limits>
#include <thread>
#include <chrono>
#include <iomanip>
#include <algorithm>

#include "CNPValidator.hpp"
#include "ServiceManager.hpp"
#include "RepairRequestFactory.hpp"
#include "EmployeeFactory.hpp"
#include "AddressFactory.hpp"
#include "ApplianceFactory.hpp"

class SimpleMenu {
private:
    ServiceManager& manager;
    RepairRequestFactory& requestFactory;
    bool running;
    
public:
    SimpleMenu(ServiceManager& mgr, RepairRequestFactory& reqFactory);
    
    void start();
    
private:
    // Main menu
    void displayMainMenu();
    void handleMainMenu();
    void handleEmployeeMenu();
    void handleRepairRequestMenu();
    void searchEmployeeByCNP();
    void modifyEmployeeName();
    void removeEmployee();
    void listRequestsByStatus();
    void generateReports();
    
    // Employee management
    void addEmployee();
    void addTechnician();
    void addReceptionist();
    void addSupervisor();
    void listEmployees();
    
    // Repair request management
    void addRepairRequest();
    void listRepairRequests();
    
    // Simulation
    void runSimulation();
    
    // Data management
    void saveData();
    void loadData();
    
    // Statistics
    void showStatistics();
    
    // Helper functions
    Address inputAddress();
    std::unique_ptr<Appliance> inputAppliance();
    std::chrono::year_month_day inputDate();
    
    void clearScreen();
    void pause();
    int getIntInput(const std::string& prompt);
    std::string getStringInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    
    std::string statusToString(Status status);
    std::string employeeTypeToString(EmployeeType type);
};