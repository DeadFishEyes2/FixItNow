#pragma once

#include <vector>
#include <string>
#include <chrono>

#include "ServiceManager.hpp"
#include "Appliance.hpp"
#include "Employee.hpp"

class EmployeeFactory;

struct Skill{
    ApplianceType type;
    std::string brand;

    Skill(ApplianceType type, std::string brand);
};

// ============================================================================
// TECHNICIAN CLASS
// ============================================================================

class Technician: public Employee{
private:

    std::vector<Skill> skills;
    std::vector<int> active_request_ids;

    Technician(
        const int id, std::string first_name, std::string last_name,
        std::string CNP, std::chrono::year_month_day date_of_hiring,
        Address residence);

    friend class EmployeeFactory;

public:

    std::vector<int> getCompletedRequests(std::chrono::year_month interval_start, std::chrono::year_month interval_end);
    int calculatePerformanceBonus() const;

    void setSalary() override;
    int getSalary() const override;
    bool canRepair(const Appliance& appliance) const;
    void addSkill(ApplianceType type, std::string brand);
    void addActiveRequest(int request_id);

    void tick();
};