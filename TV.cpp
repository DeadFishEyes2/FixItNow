#include "TV.hpp"

//Diagonal Contructor
Diagonal::Diagonal(double v, DiagonalUnit u) : value(v), unit(u) {}

// Convert to string for display
std::string Diagonal::toString() const {
    return std::to_string(value) + (unit == DiagonalUnit::Centimeters ? " cm" : " inches");
}

TV::TV(
    const std::string& brand,
    const std::string& model,
    int year,
    int price,
    const Diagonal& diagonal)
:   Appliance(
        ApplianceType::TV,
        brand,
        model,
        year,
        price),
    diagonal(diagonal)
{}

const Diagonal& TV::getDiagonal() const { return diagonal; }

std::string TV::getSpecificDetails() const {
    return "Diagonal: " + diagonal.toString();
}

//Overwrites
void TV::display() const {
    Appliance::display();
    std::cout << "Diagonal: " << diagonal.toString() << "\n";
}

std::unique_ptr<Appliance> TV::clone() const {
    return std::make_unique<TV>(*this);
}