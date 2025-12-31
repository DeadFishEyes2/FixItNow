#pragma once

#include <memory>
#include <string>
#include <chrono>
#include <vector>

#include "Employee.hpp"
#include "Technician.hpp"
#include "Receptionist.hpp"
#include "Supervisor.hpp"
#include "Address.hpp"

class EmployeeFactory {
private:
    static int next_id;
    
    EmployeeFactory() = delete;

public:
    static void initializeFactory(int saved_next_id);
    
    static std::unique_ptr<Employee> createReceptionist(
        std::string first_name,
        std::string last_name,
        std::string CNP,
        std::chrono::year_month_day date_of_hiring,
        Address residence
    );
    
    static std::unique_ptr<Employee> createTechnician(
        std::string first_name,
        std::string last_name,
        std::string CNP,
        std::chrono::year_month_day date_of_hiring,
        Address residence
    );
    
    static std::unique_ptr<Employee> createSupervisor(
        std::string first_name,
        std::string last_name,
        std::string CNP,
        std::chrono::year_month_day date_of_hiring,
        Address residence
    );

    static std::unique_ptr<Employee> createReceptionistWithID(
        int id,
        std::string first_name,
        std::string last_name,
        std::string CNP,
        std::chrono::year_month_day date_of_hiring,
        Address residence
    );

    static std::unique_ptr<Employee> createTechnicianWithID(
        int id,
        std::string first_name,
        std::string last_name,
        std::string CNP,
        std::chrono::year_month_day date_of_hiring,
        Address residence
    );

    static std::unique_ptr<Employee> createSupervisorWithID(
        int id,
        std::string first_name,
        std::string last_name,
        std::string CNP,
        std::chrono::year_month_day date_of_hiring,
        Address residence
    );
};