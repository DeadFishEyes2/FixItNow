#include "RepairRequest.hpp"

RepairRequest::RepairRequest(
    int id,
    std::unique_ptr<Appliance> appliance,
    std::chrono::system_clock::time_point timestamp,
    int complexity,
    int repair_duration,
    int remaining_time,
    int price,
    int receptionist_id)
:   id(id),
    appliance(std::move(appliance)),
    timestamp(timestamp),
    complexity(complexity),
    repair_duration(repair_duration),
    remaining_time(remaining_time),
    price(price),
    status(Status::Received),
    technician_id(0),
    receptionist_id(receptionist_id)
{}

//Getters
int RepairRequest::getId() const { return id; }
const Appliance& RepairRequest::getAppliance() const { return *appliance; }
int RepairRequest::getPrice() const { return price; }
int RepairRequest::getReceptionistId() const { return receptionist_id; }
int RepairRequest::getTechnicianId() const { return technician_id; }

//Setters
void RepairRequest::setTechnicianId(int new_technician_id) { technician_id = new_technician_id; }
void RepairRequest::setRemainingTime(int new_remaining_time) { remaining_time = new_remaining_time; }
void RepairRequest::setStatus(Status new_status) { status = new_status; }

//Checkers
bool RepairRequest::isCompleted() { return status == Status::Completed; }

bool RepairRequest::tick() { 
    remaining_time--;
    if (remaining_time == 0){
        status = Status::Completed;
    }
    return remaining_time; 
}

