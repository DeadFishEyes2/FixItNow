#include "Technician.hpp"

Skill::Skill(ApplianceType type, std::string brand): type(type), brand(brand){};

Technician::Technician(
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
    EmployeeType::Technician)
{}

bool Technician::canRepair(const Appliance& appliance) const {
    ApplianceType app_type = appliance.getType();
    std::string app_brand = appliance.getBrand();
    
    for (const auto& skill : skills) {
        if (skill.type == app_type && skill.brand == app_brand) {
            return true;
        }
    }
    
    return false;
}

void Technician::addSkill(ApplianceType type, std::string brand) {
    skills.push_back(Skill(type, brand));
}

void Technician::addActiveRequest(int request_id) {
    active_request_ids.push_back(request_id);
}

void Technician::tick() {
    ServiceManager& manager = ServiceManager::getInstance();
    
    for (auto it = active_request_ids.begin(); it != active_request_ids.end(); ) {
        RepairRequest* request = manager.findRepairRequestById(*it);
        
        if (request) {
            request->tick(); // Decrement remaining time
            
            if (request->isCompleted()) {
                //I need an observer here
                it = active_request_ids.erase(it);
            } else {
                ++it;
            }
        } else {
            // Request not found, remove from list
            // This will be an exception, something quite bad happened if this line is run
            it = active_request_ids.erase(it);
        }
    }
}

// Add these to Technician.cpp
void Technician::setSalary() {
    Employee::setSalary(); // Start with base salary logic
    salary += calculatePerformanceBonus();
}

int Technician::getSalary() const {
    return salary;
}

int Technician::calculatePerformanceBonus() const {
    // Placeholder implementation to satisfy the linker
    return 0; 
}

std::vector<int> Technician::getCompletedRequests(std::chrono::year_month interval_start, std::chrono::year_month interval_end) {
    // Placeholder implementation to satisfy the linker
    return {};
}