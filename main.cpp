#include <iostream>
#include "ApplianceFactory.hpp"

int main() {
    std::cout << "=== Testing ApplianceFactory ===" << std::endl << std::endl;
    
    // Test 1: Create TV with centimeters
    std::cout << "Test 1: Creating TV with diagonal in centimeters" << std::endl;
    auto tv1 = ApplianceFactory::createApplianceTV(
        "Samsung",
        "QN90C",
        2023,
        2500,
        139.7,
        DiagonalUnit::Centimeters
    );
    
    tv1->display();
    std::cout << std::endl;
    
    // Test 2: Create TV with inches
    std::cout << "Test 2: Creating TV with diagonal in inches" << std::endl;
    auto tv2 = ApplianceFactory::createApplianceTV(
        "LG",
        "OLED55C3",
        2023,
        1800,
        55.0,
        DiagonalUnit::Inches
    );
    
    tv2->display();
    std::cout << std::endl;
    
    // Test 3: Test getters
    std::cout << "Test 3: Testing getters" << std::endl;
    std::cout << "Brand: " << tv1->getBrand() << std::endl;
    std::cout << "Model: " << tv1->getModel() << std::endl;
    std::cout << "Year: " << tv1->getYear() << std::endl;
    std::cout << "Price: " << tv1->getCatalogPrice() << " RON" << std::endl;
    std::cout << "Age: " << tv1->getAge() << " years" << std::endl;
    std::cout << "Type: " << applianceTypeToString(tv1->getType()) << std::endl;
    std::cout << std::endl;
    
    // Test 4: Test getSpecificDetails
    std::cout << "Test 4: Testing getSpecificDetails()" << std::endl;
    std::cout << tv1->getSpecificDetails() << std::endl;
    std::cout << tv2->getSpecificDetails() << std::endl;
    std::cout << std::endl;
    
    // Test 5: Test clone
    std::cout << "Test 5: Testing clone()" << std::endl;
    auto tv1_clone = tv1->clone();
    std::cout << "Original:" << std::endl;
    tv1->display();
    std::cout << "\nClone:" << std::endl;
    tv1_clone->display();
    std::cout << std::endl;
    
    // Test 6: Test matches
    std::cout << "Test 6: Testing matches()" << std::endl;
    std::cout << "tv1 matches tv2? " << (tv1->matches(*tv2) ? "Yes" : "No") << std::endl;
    std::cout << "tv1 matches its clone? " << (tv1->matches(*tv1_clone) ? "Yes" : "No") << std::endl;
    std::cout << "tv1 matches (TV, Samsung, QN90C)? " 
              << (tv1->matches(ApplianceType::TV, "Samsung", "QN90C") ? "Yes" : "No") << std::endl;
    std::cout << std::endl;
    
    // Test 7: Polymorphism - store in base class pointer
    std::cout << "Test 7: Testing polymorphism" << std::endl;
    std::vector<std::unique_ptr<Appliance>> appliances;
    appliances.push_back(ApplianceFactory::createApplianceTV(
        "Sony", "X90K", 2022, 1500, 50.0, DiagonalUnit::Inches
    ));
    appliances.push_back(ApplianceFactory::createApplianceTV(
        "Panasonic", "HZ2000", 2021, 3000, 165.1, DiagonalUnit::Centimeters
    ));
    
    for (size_t i = 0; i < appliances.size(); ++i) {
        std::cout << "Appliance " << (i + 1) << ":" << std::endl;
        appliances[i]->display();
        std::cout << std::endl;
    }
    
    std::cout << "=== All tests completed ===" << std::endl;
    
    return 0;
}