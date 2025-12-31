#include "Address.hpp"

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

// Getters
const std::string& Address::getCountry() const { return country; }
const std::string& Address::getCity() const { return city; }
const std::string& Address::getCounty() const { return county; }
const std::string& Address::getStreet() const { return street; }
const std::string& Address::getNumber() const { return number; }

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