#include "TV.hpp"

//Diagonal Contructor
Diagonal::Diagonal(double v, DiagonalUnit u) : value(v), unit(u) {}

// Convert to string for display
std::string Diagonal::toString() const {
    return std::to_string(value) + (unit == DiagonalUnit::Centimeters ? " cm" : " inches");
}

//private constructor accessed by the ApplianceFactory class through the createApplianceTV method
TV::TV(
    std::string brand,
    std::string model,
    int year,
    int price,
    Diagonal diagonal)
:   Appliance(
        ApplianceType::TV,
        std::move(brand),
        std::move(model),
        year,
        price),
    diagonal(std::move(diagonal))
{}

const Diagonal& TV::getDiagonal() const { return diagonal; }

std::string TV::getSpecificDetails() const {
    return "Diagonal: " + diagonal.toString();
}

//Overwrites
void TV::display() const {
    Appliance::display();
    std::cout << getSpecificDetails() << "\n";
}

std::unique_ptr<Appliance> TV::clone() const {
    return std::make_unique<TV>(*this);
}