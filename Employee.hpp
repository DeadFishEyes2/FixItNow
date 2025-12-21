#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <string>
#include "Address.hpp"

enum class EmployeeType{
    Receptionist,
    Technician,
    Supervisor
};

class Employee{
    protected:
        const int id;
        std::string first_name;
        std::string surname;
        std::string CNP;
        std::chrono::year_month_day date_of_hiring;
        Address residence;
        int salary;
        EmployeeType type;

        static const int BASE_SALARY = 4000;
        static const int TRANSPORT_BONUS = 400;

    public:
        
        int getNumYearsWorked() const;

        //Calculate Bonuses
        int calculateLoyaltyBonus() const;
        int calculateTransportBonus() const;

        virtual void setSalary() = 0;
        virtual int getSalary() const = 0;
};