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