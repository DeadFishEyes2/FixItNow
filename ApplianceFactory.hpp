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
};