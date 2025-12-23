#pragma once
#include <string>
#include <memory>
#include <variant>
#include <optional>
#include <stdexcept>
#include <iostream>
#include <ostream>

#include "Address.hpp"

class AddressFactory{
    private:
        AddressFactory() = delete;
    public:
        static std::unique_ptr<Address> createAddressHouse(
            std::string country,
            std::string county,
            std::string city,
            std::string street,
            std::string number
        );

        static std::unique_ptr<Address> createAddressApartment(
            std::string country,
            std::string county,
            std::string city,
            std::string street,
            std::string number,
            std::string apt_building,
            std::string apt_floor,
            std::string apt_number
        );
};