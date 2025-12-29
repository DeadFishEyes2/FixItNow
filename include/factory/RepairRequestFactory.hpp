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
};