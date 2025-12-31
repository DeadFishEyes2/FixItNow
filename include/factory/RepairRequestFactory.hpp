#pragma once

#include "RepairRequest.hpp"

class RepairRequestFactory{
private:
    static int next_id;

public:
    RepairRequestFactory(int saved_next_id);
    void initializeFactory(int saved_next_id);

    std::unique_ptr<RepairRequest> createRepairRequest(
        std::unique_ptr<Appliance> appliance,
        int complexity,
        int remaining_time,
        int receptionist_id
    );

    std::unique_ptr<RepairRequest> createRepairRequestFromCSV(
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
    );
};