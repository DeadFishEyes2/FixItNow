#pragma once

#include "Appliance.hpp"

//Declaring the ApplianceFactory here so it can be a friend function 
class ApplianceFactory;

// ============================================================================
// WashingMachine CLASS
// ============================================================================

class WashingMachine: public Appliance{
private:
    double capacity;
    
    WashingMachine(
        std::string brand,
        std::string model,
        int year,
        int price,
        double capacity);

    friend class ApplianceFactory;

public:

    double getCapacity() const;
    
    std::string getSpecificDetails() const override;

    void display() const override;

    std::unique_ptr<Appliance> clone() const override;

};