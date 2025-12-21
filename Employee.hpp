#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <string>
#include "Adress.hpp"

enum class EmployeeType{
    Receptionist,
    Technician,
    Supervisor
};

class Employee{
    private:
        Employee() = default;
    protected:
        const int id;
        std::string first_name;
        std::string surname;
        std::string CNP;
        std::chrono::year_month_day date_of_hiring;
        Address residence;
        int salary;
        EmployeeType type;
    public:

};