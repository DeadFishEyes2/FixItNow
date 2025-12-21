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
    static std::unique_ptr<Appliance> createAppliance(
        ApplianceType type,
        const std::string& brand,
        const std::string& model,
        int year,
        int price,
        // Type-specific parameters
        const std::optional<bool>& hasFreezer = std::nullopt,
        const std::optional<Diagonal>& diagonal = std::nullopt,
        const std::optional<int>& capacity = std::nullopt
    ) {
        switch (type) {
        
            case ApplianceType::TV: {
                if (!diagonal.has_value()) {
                    throw std::invalid_argument("TV requires diagonal specification");
                }
                return std::make_unique<TV>(brand, model, year, price, diagonal.value());
            }
            
            default:
                throw std::invalid_argument("Unknown appliance type");
        }
    }
};