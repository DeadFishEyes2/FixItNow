#pragma once

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>

#include "EmployeeFactory.hpp"
#include "AddressFactory.hpp"
#include "RepairRequestFactory.hpp"

// ============================================================================
// SERVICE MANAGER - SINGLETON PATTERN
// ============================================================================

class ServiceManager {
private:
    // Delete copy constructor and assignment operator
    ServiceManager(const ServiceManager&) = delete;
    ServiceManager& operator=(const ServiceManager&) = delete;

    std::vector<std::unique_ptr<Employee>> employees;
    std::vector<std::unique_ptr<RepairRequest>> repair_requests;

public:

    ServiceManager() = default;

    static ServiceManager& getInstance() {
        static ServiceManager instance;
        return instance;
    }

    // Employee Management
    void addEmployee(std::unique_ptr<Employee> employee);
    void removeEmployeeById(int id);
    Employee* findEmployeeById(int id);
    Employee* findEmployeeByCNP(std::string CNP);

    // RepairRequest Management
    void addRepairRequest(std::unique_ptr<RepairRequest> repair_request);
    RepairRequest* findRepairRequestById(int id) const;
    
    // Filter
    std::vector<int> filter(
        Status status = Status::Pending,
        std::chrono::system_clock::time_point start = std::chrono::system_clock::time_point::min(),
        std::chrono::system_clock::time_point end = std::chrono::system_clock::time_point::max(),
        int technician_id = 0,
        int receptionist_id = 0);

    bool canRepair(int repair_request_id) const;
    void autoAssignRequests();
    void checkReceived();
    void tick();
    const std::vector<std::unique_ptr<Employee>>& getEmployees() const;
    const std::vector<std::unique_ptr<RepairRequest>>& getRepairRequests() const;
    
    // Save and Load functions
    bool saveToCSV(const std::string& employees_file = "Employees.csv", 
                   const std::string& requests_file = "RepairRequests.csv");
    bool loadFromCSV(RepairRequestFactory& requestFactory,
                     const std::string& employees_file = "Employees.csv", 
                     const std::string& requests_file = "RepairRequests.csv");
    
    // Reports
    bool generateTop3SalariesReport(const std::string& filename);
    bool generateLongestRepairReport(const std::string& filename);
    bool generatePendingRequestsReport(const std::string& filename);

private:
    // Helper functions for CSV operations
    bool saveEmployeesToCSV(const std::string& filename);
    bool saveRepairRequestsToCSV(const std::string& filename);
    bool loadEmployeesFromCSV(const std::string& filename);
    bool loadRepairRequestsFromCSV(RepairRequestFactory& requestFactory, const std::string& filename);
    
    // Helper to escape CSV fields
    std::string escapeCSV(const std::string& field);
    std::vector<std::string> parseCSVLine(const std::string& line);
    std::string employeeTypeToString(EmployeeType type) const;
};