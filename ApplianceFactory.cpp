#include "ApplianceFactory.hpp"

std::unique_ptr<Appliance> ApplianceFactory::createApplianceTV(
    std::string brand,
    std::string model,
    int year,
    int price,
    double diagonal_value,
    DiagonalUnit diagonal_unit)
{
    Diagonal diagonal {diagonal_value, diagonal_unit};

    return std::make_unique<TV> (TV(
        std::move(brand), std::move(model),
        year, price, std::move(diagonal)
    ));
};