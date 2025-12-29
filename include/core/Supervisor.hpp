#pragma once

#include "Employee.hpp"

class EmployeeFactory;

class Supervisor: public Employee {
private:
    Supervisor(
        const int id, std::string first_name, std::string last_name,
        std::string CNP, std::chrono::year_month_day date_of_hiring,
        Address residence);

    friend class EmployeeFactory;
};