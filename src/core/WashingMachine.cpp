#include "WashingMachine.hpp"

WashingMachine::WashingMachine(
    std::string brand,
    std::string model,
    int year,
    int price,
    double capacity)
:   Appliance(
        ApplianceType::WashingMachine,
        std::move(brand),
        std::move(model),
        year,
        price),
    capacity(capacity)
{}

double WashingMachine::getCapacity() const { return capacity; }

std::string WashingMachine::getSpecificDetails() const {
    return  "Washing Machine has a capacity of " + std::to_string(capacity) + "kg";
}

void WashingMachine::display() const{
    Appliance::display();
    std::cout << getSpecificDetails() << "\n";
}

std::unique_ptr<Appliance> WashingMachine::clone() const {
    return std::make_unique<WashingMachine>(*this);
}