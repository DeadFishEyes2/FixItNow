#include "EmployeeFactory.hpp"

int EmployeeFactory::next_id = 0;

void EmployeeFactory::initializeFactory(int saved_next_id) {
    next_id = saved_next_id;
}

std::unique_ptr<Employee> EmployeeFactory::createReceptionist(
    std::string first_name,
    std::string last_name,
    std::string CNP,
    std::chrono::year_month_day date_of_hiring,
    Address residence
) {
    return std::make_unique<Receptionist> (Receptionist(
        next_id++, 
        std::move(first_name), 
        std::move(last_name), 
        std::move(CNP), 
        date_of_hiring, 
        std::move(residence)
    ));
}

std::unique_ptr<Employee> EmployeeFactory::createTechnician(
    std::string first_name,
    std::string last_name,
    std::string CNP,
    std::chrono::year_month_day date_of_hiring,
    Address residence
) {
    return std::make_unique<Technician> (Technician(
        next_id++, 
        std::move(first_name), 
        std::move(last_name), 
        std::move(CNP), 
        date_of_hiring, 
        std::move(residence)
    ));
}

std::unique_ptr<Employee> EmployeeFactory::createSupervisor(
    std::string first_name,
    std::string last_name,
    std::string CNP,
    std::chrono::year_month_day date_of_hiring,
    Address residence
) {
    return std::make_unique<Supervisor> (Supervisor(
        next_id++, 
        std::move(first_name), 
        std::move(last_name), 
        std::move(CNP), 
        date_of_hiring, 
        std::move(residence)
    ));
}