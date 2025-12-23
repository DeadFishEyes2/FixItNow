#include "AddressFactory.hpp"

std::unique_ptr<Address> AddressFactory::createAddressHouse(
    std::string country,
    std::string county,
    std::string city,
    std::string street,
    std::string number)
{
    HouseDetails house{};
    return std::make_unique<Address> (Address(
        std::move(country), std::move(county), std::move(city),
        std::move(street), std::move(number), std::move(house)
    ));
}

std::unique_ptr<Address> AddressFactory::createAddressApartment(
    std::string country,
    std::string county,
    std::string city,
    std::string street,
    std::string number,
    std::string apt_building,
    std::string apt_floor,
    std::string apt_number)
{
    ApartmentDetails apartment {apt_building, apt_floor, apt_number};
    return std::make_unique<Address> (Address(
        std::move(country), std::move(county), std::move(city),
        std::move(street), std::move(number), std::move(apartment)
    ));
}