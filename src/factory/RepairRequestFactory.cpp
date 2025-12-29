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