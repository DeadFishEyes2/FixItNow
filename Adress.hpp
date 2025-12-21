#pragma once
#include <string>
#include <variant>
#include <optional>
#include <stdexcept>
#include <iostream>
#include <ostream>

// Define the enum for address types
enum class AddressType {
    House,
    Apartment
};

struct HouseDetails {};

struct ApartmentDetails {
    std::string building;
    std::string floor;
    std::string apartment_number;
};

class Address {
private:
    std::string country;
    std::string county;
    std::string city;
    std::string street;
    std::string number;
    std::variant<HouseDetails, ApartmentDetails> details;

    Address(std::string country, std::string county, std::string city,
            std::string street, std::string number, 
            std::variant<HouseDetails, ApartmentDetails> details);

public:
    // Factory method using enum instead of string
    static Address fromData(
        std::string country,
        std::string county,
        std::string city,
        std::string street,
        std::string number,
        AddressType type,  
        const std::optional<std::string>& apt_building = std::nullopt,
        const std::optional<std::string>& apt_floor = std::nullopt,
        const std::optional<std::string>& apt_number = std::nullopt);

    // Getters
    const std::string& getCountry() const;
    const std::string& getCity() const;
    
    // Return the type as enum
    AddressType getType() const;
    
    bool isApartment() const;
    
    const ApartmentDetails* getApartmentDetails() const;

    //Log the data into the console
    void log(std::ostream& stream = std::cout) const;
};

AddressType stringToAddressType(const std::string& str);
std::string addressTypeToString(AddressType type);