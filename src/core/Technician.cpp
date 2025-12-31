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

void Technician::setSalary() {
    Employee::setSalary(); // Start with base salary logic
    salary += calculatePerformanceBonus();
}

int Technician::getSalary() const {
    return salary;
}

std::vector<int> Technician::getCompletedRequests(std::chrono::year_month interval_start, std::chrono::year_month interval_end) const {

    auto start_tp = std::chrono::sys_days{interval_start / 1};
    
    auto end_tp = std::chrono::sys_days{(interval_end + std::chrono::months{1}) / 1};

    return ServiceManager::getInstance().filter(
        Status::Completed,
        start_tp,
        end_tp,
        this->getId(),
        0 // Ignore receptionist filter
    );
}

int Technician::calculatePerformanceBonus() const {

    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::year_month_day{std::chrono::floor<std::chrono::days>(now)};
    
    auto current_month = today.year() / today.month();
    auto last_month = current_month - std::chrono::months{1};

    std::vector<int> completed_ids = getCompletedRequests(last_month, last_month);

    int performance_bonus = 0;

    for (auto completed_id : completed_ids){
        performance_bonus += (ServiceManager::getInstance().findRepairRequestById(completed_id)->getPrice())*0.02;
    }

    return performance_bonus;
}

int Technician::getActiveRequestsCount() const { return active_request_ids.size(); }
const std::vector<int> Technician::getActiveRequests() const { return active_request_ids; }

const std::vector<Skill>& Technician::getSkills() const { return skills; }