#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <variant>
#include <optional>
#include <sstream>
#include <algorithm>

// ============================================================================
// APPLIANCE TYPE ENUM
// ============================================================================

enum class ApplianceType {
    Fridge,
    TV,
    WashingMachine
};

// Convert string to enum
ApplianceType stringToApplianceType(const std::string& str);

// Convert enum to string
std::string applianceTypeToString(ApplianceType type);

// ============================================================================
// BASE APPLIANCE CLASS
// ============================================================================

class Appliance {
protected:
    ApplianceType type;           // Fridge/TV/WashingMachine
    std::string brand;            // "Samsung", "LG"
    std::string model;            // "Frost200", "SmartTV55"
    int year_of_manufacture;      // 2020
    int catalog_price;             // 1500
    
    // Protected constructor
    Appliance(ApplianceType type, const std::string& brand, const std::string& model,
              int year, int price);
    
public:
    virtual ~Appliance() = default;
    
    // Getters
    ApplianceType getType() const;
    std::string getBrand() const;
    std::string getModel() const;
    int getYear() const;
    int getCatalogPrice() const;
    
    // Calculate age (for repair duration calculation)
    int getAge() const;
    
    // Virtual display method - will get overwritten by derived classes to display specific details
    virtual void display() const;
    
    // Pure virtual - must be implemented by derived classes since Appliance has to specific details
    virtual std::string getSpecificDetails() const = 0;
    
    // Clone method for copying appliances
    virtual std::unique_ptr<Appliance> clone() const = 0;
    
    // Check if 2 appliances are the same, used to check if a technician can fix the appliance
    bool matches(ApplianceType other_type, const std::string& other_brand, const std::string& other_model) const;
    bool matches(const Appliance& other) const;
};
