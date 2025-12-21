#include <string>
#include <variant>
#include <optional>
#include <stdexcept>
#include <iostream>
#include <ostream>
#include "Adress.hpp"

//Private constructor
Address::Address(
    std::string country,
    std::string county,
    std::string city,
    std::string street,
    std::string number,
    std::variant<HouseDetails, ApartmentDetails> details)
:   country(std::move(country)),
    county(std::move(county)),
    city(std::move(city)),
    street(std::move(street)),
    number(std::move(number)),
    details(std::move(details)) 
{}

// Factory method using enum instead of string
Address Address::fromData(
    std::string country,
    std::string county,
    std::string city,
    std::string street,
    std::string number,
    AddressType type,  // ✅ Enum instead of string
    const std::optional<std::string>& apt_building,
    const std::optional<std::string>& apt_floor,
    const std::optional<std::string>& apt_number)
{
    switch (type) {  // Cleaner than if-else for enums
        case AddressType::Apartment: {
            if (!apt_building || !apt_floor || !apt_number) {
                throw std::invalid_argument(
                    "Apartment address requires building, floor, and number"
                );
            }
            ApartmentDetails apt{*apt_building, *apt_floor, *apt_number};
            return Address(
                std::move(country), std::move(county), std::move(city),
                std::move(street), std::move(number), std::move(apt)
            );
        }
        case AddressType::House: {
            HouseDetails house{};
            return Address(
                std::move(country), std::move(county), std::move(city),
                std::move(street), std::move(number), std::move(house)
            );
        }
    }
    // If you add a new enum value and forget to handle it, 
    // compiler will warn (with -Wswitch)
    throw std::invalid_argument("Unknown address type");
}

// Getters
const std::string& Address::getCountry() const {
    return country;
}
const std::string& Address::getCity() const {
    return city;
}

// Return the type as enum
AddressType Address::getType() const {
    // std::holds_alternative checks which type the variant currently holds
    if (std::holds_alternative<ApartmentDetails>(details)) {
        return AddressType::Apartment;
    } else {
        return AddressType::House;
    }
}

bool Address::isApartment() const {
    return std::holds_alternative<ApartmentDetails>(details);
}

const ApartmentDetails* Address::getApartmentDetails() const {
    return std::get_if<ApartmentDetails>(&details);
}

void Address::log(std::ostream& stream) const{
    stream << "--- Address information ---\n";
    stream << "Country " << country << '\n';
    stream << "County " << county << '\n';
    stream << "City " << city << '\n';
    stream << "Street " << street << '\n';
    stream << "Number " << number << '\n';
    if (const auto* aptPtr = getApartmentDetails()) {
        stream << "Type: Apartment\n";
        stream << "Building: " << aptPtr->building << '\n';
        stream << "Floor: " << aptPtr->floor << '\n';
        stream << "Apartment: " << aptPtr->apartment_number << '\n';
    } else {
        stream << "Type: House\n";
    }
}

// Convert string from CSV/JSON to enum
AddressType stringToAddressType(const std::string& str) {
    if (str == "house") return AddressType::House;
    if (str == "apartment") return AddressType::Apartment;
    throw std::invalid_argument("Invalid address type string: " + str);
}

// Convert enum to string for CSV/JSON
std::string addressTypeToString(AddressType type) {
    switch (type) {
        case AddressType::House: return "house";
        case AddressType::Apartment: return "apartment";
    }
    throw std::invalid_argument("Unknown address type");
}