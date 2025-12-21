#include <iostream>
#include <memory>
#include "TV.hpp"
#include "AddressFactory.hpp"

int main() {
    std::cout << "=== Testing Address Factory ===\n\n";

    // Test 1: Create a house address
    std::cout << "--- House Address ---\n";
    auto house = AddressFactory::createAddressHouse(
        "USA",
        "Los Angeles County",
        "Los Angeles",
        "Sunset Boulevard",
        "1234"
    );
    house->log();
    std::cout << "Type: " << addressTypeToString(house->getType()) << "\n";
    std::cout << "Is Apartment? " << (house->isApartment() ? "Yes" : "No") << "\n\n";

    // Test 2: Create an apartment address
    std::cout << "--- Apartment Address ---\n";
    auto apartment = AddressFactory::createAddressApartment(
        "Romania",
        "Bucharest",
        "Bucharest",
        "Calea Victoriei",
        "42",
        "Building A",
        "5th Floor",
        "Apt 12"
    );
    apartment->log();
    std::cout << "Type: " << addressTypeToString(apartment->getType()) << "\n";
    std::cout << "Is Apartment? " << (apartment->isApartment() ? "Yes" : "No") << "\n";
    
    // Get apartment details
    if (const auto* details = apartment->getApartmentDetails()) {
        std::cout << "Apartment Details:\n";
        std::cout << "  Building: " << details->building << "\n";
        std::cout << "  Floor: " << details->floor << "\n";
        std::cout << "  Apartment Number: " << details->apartment_number << "\n";
    }
    std::cout << "\n";

    // Test 3: Create multiple addresses and store in a vector
    std::cout << "--- Multiple Addresses ---\n";
    std::vector<std::unique_ptr<Address>> addresses;
    
    addresses.push_back(AddressFactory::createAddressHouse(
        "UK", "Greater London", "London", "Baker Street", "221B"
    ));
    
    addresses.push_back(AddressFactory::createAddressApartment(
        "France", "Île-de-France", "Paris", "Champs-Élysées", "100",
        "Tower B", "10", "Suite 1001"
    ));
    
    addresses.push_back(AddressFactory::createAddressHouse(
        "Germany", "Bavaria", "Munich", "Marienplatz", "8"
    ));

    for (size_t i = 0; i < addresses.size(); ++i) {
        std::cout << "Address " << (i + 1) << ":\n";
        addresses[i]->log();
        std::cout << "\n";
    }

    // Test 4: Test getters
    std::cout << "--- Testing Getters ---\n";
    std::cout << "House country: " << house->getCountry() << "\n";
    std::cout << "House city: " << house->getCity() << "\n";
    std::cout << "Apartment country: " << apartment->getCountry() << "\n";
    std::cout << "Apartment city: " << apartment->getCity() << "\n\n";

    // Test 5: Demonstrate ownership transfer
    std::cout << "--- Ownership Transfer ---\n";
    std::unique_ptr<Address> movedAddress = std::move(house);
    std::cout << "Address moved successfully\n";
    movedAddress->log();
    // house is now nullptr and cannot be used

    return 0;
}
