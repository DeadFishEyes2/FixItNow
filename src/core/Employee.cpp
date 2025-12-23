#include "Employee.hpp"

int Employee::calculateTransportBonus() const{
    if (residence.getCity() != "Bucharest")
        return 400;
    return 0;
}

int Employee::getNumYearsWorked() const {
    using namespace std::chrono;

    const auto today_tp = floor<days>(system_clock::now());

    const year_month_day today{ today_tp };

    int years = int(today.year()) - int(date_of_hiring.year());

    if (
        today.month() < date_of_hiring.month() ||
        (today.month() == date_of_hiring.month() &&
         today.day() < date_of_hiring.day())
    ) {
        years--;
    }

    return years;
}

int Employee::calculateLoyaltyBonus() const{
    int yearsWorked = getNumYearsWorked();
    int loyaltyPeriods = yearsWorked / 3;
    return BASE_SALARY * 0.05 * loyaltyPeriods; 
}

void Employee::setSalary(){
    int loyalty_bonus = calculateLoyaltyBonus();
    int transport_bonus = calculateTransportBonus();
    salary = BASE_SALARY + loyalty_bonus + transport_bonus;
}

int Employee::getSalary() const { return salary; }

Employee::Employee(
    const int id,
    std::string first_name,
    std::string last_name,
    std::string CNP,
    std::chrono::year_month_day date_of_hiring,
    Address residence,
    EmployeeType type)
:   id(id),
    first_name(std::move(first_name)),
    last_name(std::move(last_name)),
    CNP(std::move(CNP)),
    date_of_hiring(std::move(date_of_hiring)),
    residence(std::move(residence)),
    type(type)
{}