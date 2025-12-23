#include "Fridge.hpp"

Fridge::Fridge(
    std::string brand,
    std::string model,
    int year,
    int price,
    bool has_freezer)
:   Appliance(
        ApplianceType::Fridge,
        std::move(brand),
        std::move(model),
        year,
        price),
    has_freezer(has_freezer)
{}

bool Fridge::hasFreezer() const { return has_freezer; }

std::string Fridge::getSpecificDetails() const {
    if (has_freezer)
        return "Fridge has a freezer";
    return "Fridge doesn't have a freezer";
}

void Fridge::display() const{
    Appliance::display();
    std::cout << getSpecificDetails() << "\n";
}

std::unique_ptr<Appliance> Fridge::clone() const {
    return std::make_unique<Fridge>(*this);
}