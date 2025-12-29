#pragma once

#include <vector>

#include "Employee.hpp"

class EmployeeFactory;

class Receptionist: public Employee {
private:
    std::vector<int> received_repair_requests;

    Receptionist(
        const int id, std::string first_name, std::string last_name,
        std::string CNP, std::chrono::year_month_day date_of_hiring,
        Address residence);

    friend class EmployeeFactory;

public:

    //Setters
    void setReceivedRepairRequests(std::vector<int> new_received_repair_requests);
    void addReceivedRepairRequest(int new_received_repair_request);

    //Getters
    std::vector<int> getReceivedRepairRequests();

};