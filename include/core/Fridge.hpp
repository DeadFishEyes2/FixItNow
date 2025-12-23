#pragma once

#include "Appliance.hpp"

//Declaring the ApplianceFactory here so it can be a friend function 
class ApplicanceFactory;

// ============================================================================
// FRIDGE CLASS
// ============================================================================

class Fridge: public Appliance{
private:
    bool has_freezer;
    
    Fridge(
        std::string brand,
        std::string model,
        int year,
        int price,
        bool has_freezer);

    friend class ApplianceFactory;

public:

    bool hasFreezer() const;
    
    std::string getSpecificDetails() const override;

    void display() const override;

    std::unique_ptr<Appliance> clone() const override;

};