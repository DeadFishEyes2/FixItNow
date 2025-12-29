#include "Supervisor.hpp"

Supervisor::Supervisor(
    const int id,
    std::string first_name,
    std::string last_name,
    std::string CNP,
    std::chrono::year_month_day date_of_hiring,
    Address residence)
:   Employee(
        id,
        first_name,
        last_name,
        CNP,
        date_of_hiring,
        residence,
        EmployeeType::Supervisor
    )
{}