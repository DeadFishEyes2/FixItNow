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

// ============================================================================
// BASE EMPLOYEE CLASS
// ============================================================================

class Employee{
protected:
    const int id;
    std::string first_name;
    std::string last_name;
    std::string CNP;
    std::chrono::year_month_day date_of_hiring;
    Address residence;
    int salary;
    EmployeeType type;

    static const int BASE_SALARY = 4000;
    static const int TRANSPORT_BONUS = 400;

    Employee(
        const int id, std::string first_name, std::string last_name,
        std::string CNP, std::chrono::year_month_day date_of_hiring,
        Address residence, EmployeeType type);

public:
    
    int getNumYearsWorked() const;

    //Calculate Bonuses
    int calculateLoyaltyBonus() const;
    int calculateTransportBonus() const;

    //Getters
    virtual int getSalary() const;
    int getId() const;
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    EmployeeType getType() const;
    const std::string& getCNP() const;
    const std::chrono::year_month_day& getDateOfHiring() const;
    const Address& getResidence() const;

    //Setters
    virtual void setSalary();

    void changeFirstName(std::string new_first_name);
    void changeLastName(std::string new_last_name);
};