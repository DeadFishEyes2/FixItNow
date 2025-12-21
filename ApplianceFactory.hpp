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

#include "TV.hpp"
#include "Fridge.hpp"
#include "WashingMachine.hpp"

class ApplianceFactory {
private:
    ApplianceFactory() = delete;
    
public:
    // Create appliance from parameters
    static std::unique_ptr<Appliance> createApplianceTV(
        std::string brand,
        std::string model,
        int year,
        int price,
        double diagonal_value,
        DiagonalUnit diagonal_unit
    );

    static std::unique_ptr<Appliance> createApplianceFridge(
        std::string brand,
        std::string model,
        int year,
        int price,
        bool has_freezer
    );

    static std::unique_ptr<Appliance> createApplianceWashingMachine(
        std::string brand,
        std::string model,
        int year,
        int price,
        double capacity
    );
};