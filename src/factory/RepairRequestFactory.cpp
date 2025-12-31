#include "RepairRequestFactory.hpp"

//Factory Initialization for loading the next id
int RepairRequestFactory::next_id = 0;
void RepairRequestFactory::initializeFactory(int saved_next_id) { next_id = saved_next_id; }
RepairRequestFactory::RepairRequestFactory(int saved_next_id) { initializeFactory(saved_next_id); }

std::unique_ptr<RepairRequest> RepairRequestFactory::createRepairRequest(
    std::unique_ptr<Appliance> appliance,
    int complexity,
    int remaining_time,
    int receptionist_id)
{
    int repair_duration = (appliance->getAge())*complexity;
    int price = (appliance->getAge())*(appliance->getCatalogPrice());

    return std::make_unique<RepairRequest> (RepairRequest(
        next_id++,
        std::move(appliance),
        std::chrono::system_clock::now(),
        complexity,
        repair_duration,
        remaining_time,
        price,
        receptionist_id
    ));
}

std::unique_ptr<RepairRequest> RepairRequestFactory::createRepairRequestFromCSV(
    int id,
    std::unique_ptr<Appliance> appliance,
    std::chrono::system_clock::time_point timestamp,
    int complexity,
    int repair_duration,
    int remaining_time,
    int price,
    Status status,
    int receptionist_id,
    int technician_id
) {
    // Create the repair request with all fields from CSV
    auto request = std::unique_ptr<RepairRequest>(new RepairRequest(
        id,
        std::move(appliance),
        timestamp,
        complexity,
        repair_duration,
        remaining_time,
        price,
        receptionist_id
    ));
    
    // Set additional fields that aren't in the constructor
    request->setStatus(status);
    if (technician_id != 0) {
        request->setTechnicianId(technician_id);
    }
    
    // Update next_id if this id is >= current next_id
    if (id >= next_id) {
        next_id = id + 1;
    }
    
    return request;
}