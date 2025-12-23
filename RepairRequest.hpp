#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <string>

#include "ApplianceFactory.hpp"

class RepairRequestFactory;

enum class Status{
    Received,
    Pending,
    InProgress,
    Completed,
    Rejected
};

// ============================================================================
// BASE EMPLOYEE CLASS
// ============================================================================

class RepairRequest{
private:
    int id;
    std::unique_ptr<Appliance> appliance;
    std::string timestamp;
    int complexity;
    int repair_duration;
    int remaining_time;
    int price;
    Status status;
    int receptionist_id;
    int technician_id; //if 0 then it hasn't been assigned to a technician

    RepairRequest(
        int id,
        std::unique_ptr<Appliance> appliance,
        std::string timestamp,
        int complexity,
        int repair_duration,
        int remaining_time,
        int price,
        int receptionist_id
    );

    friend class RepairRequestFactory;

public:

    //Getters
    int getId() const;
    const Appliance& getAppliance() const;
    int getPrice() const;
    Status getStatus() const;
    int getReceptionistId() const;
    int getTechnicianId() const;

    //Setters
    void setTechnicianId(int new_technician_id);
    void setRemainingTime(int new_remaining_time);

    bool tick();
};