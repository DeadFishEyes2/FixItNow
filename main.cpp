#include <chrono>
#include <iostream>
#include <iomanip>
#include "Adress.hpp"

int main() {
    Address houseAddr = Address::fromData(
        "USA", "Suffolk", "Boston", "Oak Street", "123",
        AddressType::Apartment, "A21","9","124"
    );
    houseAddr.log();
}
