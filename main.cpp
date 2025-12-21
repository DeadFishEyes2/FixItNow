#include <iostream>
#include <memory>
#include "TV.hpp"

int main() {
    try {
        // 1. Create a Diagonal object for the TV
        // 55 inches
        Diagonal myDiagonal(55.0, DiagonalUnit::Inches);

        // 2. Instantiate a TV object
        // Brand: Samsung, Model: NeoQLED, Year: 2022, Price: 4500 RON
        TV myTv("Samsung", "NeoQLED", 2022, 4500, myDiagonal);

        std::cout << "--- Testing TV Display ---" << std::endl;
        myTv.display();

        std::cout << "\n--- Testing Getters ---" << std::endl;
        std::cout << "Brand: " << myTv.getBrand() << std::endl;
        std::cout << "Age: " << myTv.getAge() << " years old" << std::endl;
        std::cout << "Specific Details: " << myTv.getSpecificDetails() << std::endl;

        // 3. Testing Cloning (Virtual Copy)
        std::cout << "\n--- Testing Clone Method ---" << std::endl;
        std::unique_ptr<Appliance> clonedTv = myTv.clone();
        std::cout << "Cloned TV Display:" << std::endl;
        clonedTv->display();

        // 4. Testing Matches logic
        std::cout << "\n--- Testing Match Logic ---" << std::endl;
        bool isMatch = myTv.matches(*clonedTv);
        std::cout << "Matches 'Samsung NeoQLED TV'? " << (isMatch ? "Yes" : "No") << std::endl;

        bool isWrongMatch = myTv.matches(ApplianceType::Fridge, "LG", "CoolerX");
        std::cout << "Matches 'LG CoolerX Fridge'? " << (isWrongMatch ? "Yes" : "No") << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
