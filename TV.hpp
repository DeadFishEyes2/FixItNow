#pragma once
#include "Appliance.hpp"

//Declaring the ApplianceFactory here so it can be a friend function 
class ApplianceFactory;

// ============================================================================
// TV SPECIFIC DETAILS
// ============================================================================

enum class DiagonalUnit {
    Centimeters,
    Inches
};

struct Diagonal {
    double value;           // 12.4
    DiagonalUnit unit;      // Centimeters/Inches
    
    //Diagonal Contructor
    Diagonal(double v, DiagonalUnit u);
    
    // Convert to string for display
    std::string toString() const;
};

// ============================================================================
// TV CLASS
// ============================================================================

class TV: public Appliance{
private:
    Diagonal diagonal;

    // Constructor - will be used in the factory method
    TV(const std::string& brand, const std::string& model,
        int year, int price, const Diagonal& diagonal);

    friend class ApplianceFactory;

public:
    
    //Specific details
    const Diagonal& getDiagonal() const;
    
    std::string getSpecificDetails() const override;
    
    //Overwrites
    void display() const override;
    
    std::unique_ptr<Appliance> clone() const;
};