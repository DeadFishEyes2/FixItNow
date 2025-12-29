#pragma once

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <chrono>

#include "EmployeeFactory.hpp"
#include "RepairRequestFactory.hpp"

// ============================================================================
// SERVICE MANAGER - SINGLETHON PATTERN
// ============================================================================

class ServiceManager {
private:
    // Private constructor for Singleton pattern
    ServiceManager() = default;

    // Delete copy constructor and assignment operator
    ServiceManager(const ServiceManager&) = delete;
    ServiceManager& operator=(const ServiceManager&) = delete;

    std::vector<std::unique_ptr<Employee>> employees;
    std::vector<std::unique_ptr<RepairRequest>> repair_requests;

public:
    static ServiceManager& getInstance() {
        static ServiceManager instance;
        return instance;
    }

    // Employee Management
    void addEmployee(std::unique_ptr<Employee> employee);
    void removeEmployeeById(int id);
    Employee* findEmployeeById(int id);

    // RepairRequest Managment
    void addRepairRequest(std::unique_ptr<RepairRequest> repair_request);
    RepairRequest* findRepairRequestById(int id);
    
    //Filter
    std::vector<int> filter(
        Status status = Status::Pending,
        std::chrono::system_clock::time_point start = std::chrono::system_clock::time_point::min(),
        std::chrono::system_clock::time_point end = std::chrono::system_clock::time_point::max(),
        int technician_id = 0,
        int receptionist_id = 0);

    void autoAssignRequests();
    void tick();
    
    // Reports
    void generateSalaryReport(const std::string& filename);
};