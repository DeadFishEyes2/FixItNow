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

std::unique_ptr<Employee> EmployeeFactory::createReceptionistWithID(
    int id,
    std::string first_name,
    std::string last_name,
    std::string CNP,
    std::chrono::year_month_day date_of_hiring,
    Address residence
) {
    // Update next_id if this id is >= current next_id
    if (id >= next_id) {
        next_id = id + 1;
    }
    
    return std::unique_ptr<Employee>(new Receptionist(
        id,
        std::move(first_name),
        std::move(last_name),
        std::move(CNP),
        date_of_hiring,
        std::move(residence)
    ));
}

std::unique_ptr<Employee> EmployeeFactory::createTechnicianWithID(
    int id,
    std::string first_name,
    std::string last_name,
    std::string CNP,
    std::chrono::year_month_day date_of_hiring,
    Address residence
) {
    // Update next_id if this id is >= current next_id
    if (id >= next_id) {
        next_id = id + 1;
    }
    
    return std::unique_ptr<Employee>(new Technician(
        id,
        std::move(first_name),
        std::move(last_name),
        std::move(CNP),
        date_of_hiring,
        std::move(residence)
    ));
}

std::unique_ptr<Employee> EmployeeFactory::createSupervisorWithID(
    int id,
    std::string first_name,
    std::string last_name,
    std::string CNP,
    std::chrono::year_month_day date_of_hiring,
    Address residence
) {
    // Update next_id if this id is >= current next_id
    if (id >= next_id) {
        next_id = id + 1;
    }
    
    return std::unique_ptr<Employee>(new Supervisor(
        id,
        std::move(first_name),
        std::move(last_name),
        std::move(CNP),
        date_of_hiring,
        std::move(residence)
    ));
}