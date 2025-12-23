#include "Appliance.hpp"

// Convert string to enum
ApplianceType stringToApplianceType(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (lower == "fridge") return ApplianceType::Fridge;
    if (lower == "tv") return ApplianceType::TV;
    if (lower == "washingmachine") return ApplianceType::WashingMachine;
    
    throw std::invalid_argument("Unknown appliance type: " + str);
}

// Convert enum to string
std::string applianceTypeToString(ApplianceType type) {
    switch (type) {
        case ApplianceType::Fridge: return "Fridge";
        case ApplianceType::TV: return "TV";
        case ApplianceType::WashingMachine: return "WashingMachine";
    }
    return "Unknown";
}


//Protected constructor
Appliance::Appliance(
    ApplianceType type,
    std::string brand,
    std::string model,
    int year,
    int price)
:   type(type),
    brand(std::move(brand)), 
    model(std::move(model)), 
    year_of_manufacture(year), 
    catalog_price(price) 
{}

// Getters
ApplianceType Appliance::getType() const { return type; }
std::string Appliance::getBrand() const { return brand; }
std::string Appliance::getModel() const { return model; }
int Appliance::getYear() const { return year_of_manufacture; }
int Appliance::getCatalogPrice() const { return catalog_price; }

// Calculate age (for repair duration calculation)
int Appliance::getAge() const {
    using namespace std::chrono;

    const auto today_tp = floor<days>(system_clock::now());

    const year_month_day today{ today_tp };

    int age = int(today.year()) - year_of_manufacture;
    return age;
}


// Check if 2 appliances are the same, used to check if a technician can fix the appliance
bool Appliance::matches(ApplianceType other_type, const std::string& other_brand, const std::string& other_model) const {
    return type == other_type && brand == other_brand && model == other_model;
}

bool Appliance::matches(const Appliance& other) const{
    return matches(other.type, other.brand, other.model);
}

void Appliance::display() const {
    std::cout << "Type: " << applianceTypeToString(type) << "\n";
    std::cout << "Brand: " << brand << "\n";
    std::cout << "Model: " << model << "\n";
    std::cout << "Year: " << year_of_manufacture << "\n";
    std::cout << "Price: " << catalog_price << " RON\n";
}