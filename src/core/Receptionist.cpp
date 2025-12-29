#include "Receptionist.hpp"

Receptionist::Receptionist(
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
        EmployeeType::Receptionist
    )
{}

void Receptionist::setReceivedRepairRequests(std::vector<int> new_received_repair_requests){
    received_repair_requests = new_received_repair_requests;
}

void Receptionist::addReceivedRepairRequest(int new_received_repair_request){
    received_repair_requests.push_back(new_received_repair_request);
}

std::vector<int> Receptionist::getReceivedRepairRequests() { return received_repair_requests; }