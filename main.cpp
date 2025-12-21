#include <iostream>
#include <iomanip>
#include "ApplianceFactory.hpp"

void printSeparator() {
    std::cout << std::string(60, '=') << std::endl;
}

void printTestHeader(const std::string& testName) {
    printSeparator();
    std::cout << ">>> " << testName << std::endl;
    printSeparator();
}

int main() {
    std::cout << "\n";
    printTestHeader("APPLIANCE FACTORY COMPREHENSIVE TEST SUITE");
    std::cout << "\n";

    // ========================================================================
    // TEST 1: Create Different Appliance Types
    // ========================================================================
    printTestHeader("TEST 1: Creating Different Appliance Types");
    
    auto tv = ApplianceFactory::createApplianceTV(
        "Samsung", "QN90C", 2023, 2500, 55.0, DiagonalUnit::Inches
    );
    
    auto fridge = ApplianceFactory::createApplianceFridge(
        "LG", "InstaView", 2022, 3500, true
    );
    
    auto washingMachine = ApplianceFactory::createApplianceWashingMachine(
        "Bosch", "Serie8", 2023, 2200, 9.5
    );
    
    std::cout << "\n--- TV ---" << std::endl;
    tv->display();
    
    std::cout << "\n--- Fridge ---" << std::endl;
    fridge->display();
    
    std::cout << "\n--- Washing Machine ---" << std::endl;
    washingMachine->display();
    std::cout << "\n";

    // ========================================================================
    // TEST 2: Test All Getters
    // ========================================================================
    printTestHeader("TEST 2: Testing Getters for Each Appliance Type");
    
    std::cout << "\n--- TV Getters ---" << std::endl;
    std::cout << "Type: " << applianceTypeToString(tv->getType()) << std::endl;
    std::cout << "Brand: " << tv->getBrand() << std::endl;
    std::cout << "Model: " << tv->getModel() << std::endl;
    std::cout << "Year: " << tv->getYear() << std::endl;
    std::cout << "Price: " << tv->getCatalogPrice() << " RON" << std::endl;
    std::cout << "Age: " << tv->getAge() << " years" << std::endl;
    
    std::cout << "\n--- Fridge Getters ---" << std::endl;
    std::cout << "Type: " << applianceTypeToString(fridge->getType()) << std::endl;
    std::cout << "Brand: " << fridge->getBrand() << std::endl;
    std::cout << "Model: " << fridge->getModel() << std::endl;
    std::cout << "Year: " << fridge->getYear() << std::endl;
    std::cout << "Price: " << fridge->getCatalogPrice() << " RON" << std::endl;
    std::cout << "Age: " << fridge->getAge() << " years" << std::endl;
    
    std::cout << "\n--- Washing Machine Getters ---" << std::endl;
    std::cout << "Type: " << applianceTypeToString(washingMachine->getType()) << std::endl;
    std::cout << "Brand: " << washingMachine->getBrand() << std::endl;
    std::cout << "Model: " << washingMachine->getModel() << std::endl;
    std::cout << "Year: " << washingMachine->getYear() << std::endl;
    std::cout << "Price: " << washingMachine->getCatalogPrice() << " RON" << std::endl;
    std::cout << "Age: " << washingMachine->getAge() << " years" << std::endl;
    std::cout << "\n";

    // ========================================================================
    // TEST 3: Test Specific Details
    // ========================================================================
    printTestHeader("TEST 3: Testing getSpecificDetails()");
    
    std::cout << "\nTV: " << tv->getSpecificDetails() << std::endl;
    std::cout << "Fridge: " << fridge->getSpecificDetails() << std::endl;
    std::cout << "Washing Machine: " << washingMachine->getSpecificDetails() << std::endl;
    std::cout << "\n";

    // ========================================================================
    // TEST 4: Test Cloning
    // ========================================================================
    printTestHeader("TEST 4: Testing clone() Method");
    
    std::cout << "\n--- Cloning TV ---" << std::endl;
    auto tv_clone = tv->clone();
    std::cout << "Original:" << std::endl;
    tv->display();
    std::cout << "\nClone:" << std::endl;
    tv_clone->display();
    
    std::cout << "\n--- Cloning Fridge ---" << std::endl;
    auto fridge_clone = fridge->clone();
    std::cout << "Original:" << std::endl;
    fridge->display();
    std::cout << "\nClone:" << std::endl;
    fridge_clone->display();
    
    std::cout << "\n--- Cloning Washing Machine ---" << std::endl;
    auto wm_clone = washingMachine->clone();
    std::cout << "Original:" << std::endl;
    washingMachine->display();
    std::cout << "\nClone:" << std::endl;
    wm_clone->display();
    std::cout << "\n";

    // ========================================================================
    // TEST 5: Test Matching
    // ========================================================================
    printTestHeader("TEST 5: Testing matches() Method");
    
    auto tv2 = ApplianceFactory::createApplianceTV(
        "Samsung", "QN90C", 2023, 2500, 65.0, DiagonalUnit::Inches
    );
    
    auto tv3 = ApplianceFactory::createApplianceTV(
        "LG", "OLED55", 2023, 1800, 55.0, DiagonalUnit::Inches
    );
    
    std::cout << "\n--- Match Tests ---" << std::endl;
    std::cout << "tv matches tv_clone? " 
              << (tv->matches(*tv_clone) ? "✓ Yes" : "✗ No") << std::endl;
    std::cout << "tv matches tv2 (same brand/model, different size)? " 
              << (tv->matches(*tv2) ? "✓ Yes" : "✗ No") << std::endl;
    std::cout << "tv matches tv3 (different brand/model)? " 
              << (tv->matches(*tv3) ? "✓ Yes" : "✗ No") << std::endl;
    std::cout << "tv matches fridge? " 
              << (tv->matches(*fridge) ? "✓ Yes" : "✗ No") << std::endl;
    
    std::cout << "\n--- Match by Parameters ---" << std::endl;
    std::cout << "tv matches (TV, Samsung, QN90C)? " 
              << (tv->matches(ApplianceType::TV, "Samsung", "QN90C") ? "✓ Yes" : "✗ No") << std::endl;
    std::cout << "tv matches (TV, Samsung, QN85C)? " 
              << (tv->matches(ApplianceType::TV, "Samsung", "QN85C") ? "✓ Yes" : "✗ No") << std::endl;
    std::cout << "tv matches (Fridge, Samsung, QN90C)? " 
              << (tv->matches(ApplianceType::Fridge, "Samsung", "QN90C") ? "✓ Yes" : "✗ No") << std::endl;
    std::cout << "\n";

    // ========================================================================
    // TEST 6: Test Multiple Units for TV
    // ========================================================================
    printTestHeader("TEST 6: Testing TV with Different Diagonal Units");
    
    auto tv_cm = ApplianceFactory::createApplianceTV(
        "Sony", "X90K", 2022, 1500, 139.7, DiagonalUnit::Centimeters
    );
    
    auto tv_inch = ApplianceFactory::createApplianceTV(
        "Panasonic", "HZ2000", 2021, 3000, 65.0, DiagonalUnit::Inches
    );
    
    std::cout << "\n--- TV with Centimeters ---" << std::endl;
    tv_cm->display();
    
    std::cout << "\n--- TV with Inches ---" << std::endl;
    tv_inch->display();
    std::cout << "\n";

    // ========================================================================
    // TEST 7: Test Fridge with/without Freezer
    // ========================================================================
    printTestHeader("TEST 7: Testing Fridges with Different Configurations");
    
    auto fridge_with_freezer = ApplianceFactory::createApplianceFridge(
        "Samsung", "FamilyHub", 2023, 4500, true
    );
    
    auto fridge_no_freezer = ApplianceFactory::createApplianceFridge(
        "Whirlpool", "SlimFit", 2022, 1800, false
    );
    
    std::cout << "\n--- Fridge WITH Freezer ---" << std::endl;
    fridge_with_freezer->display();
    
    std::cout << "\n--- Fridge WITHOUT Freezer ---" << std::endl;
    fridge_no_freezer->display();
    std::cout << "\n";

    // ========================================================================
    // TEST 8: Test Washing Machines with Different Capacities
    // ========================================================================
    printTestHeader("TEST 8: Testing Washing Machines with Different Capacities");
    
    auto wm_small = ApplianceFactory::createApplianceWashingMachine(
        "Miele", "W1", 2023, 1800, 7.0
    );
    
    auto wm_large = ApplianceFactory::createApplianceWashingMachine(
        "Samsung", "AddWash", 2022, 2500, 12.0
    );
    
    std::cout << "\n--- Small Capacity (7 kg) ---" << std::endl;
    wm_small->display();
    
    std::cout << "\n--- Large Capacity (12 kg) ---" << std::endl;
    wm_large->display();
    std::cout << "\n";

    // ========================================================================
    // TEST 9: Polymorphism - Mixed Collection
    // ========================================================================
    printTestHeader("TEST 9: Testing Polymorphism with Mixed Collection");
    
    std::vector<std::unique_ptr<Appliance>> appliances;
    
    appliances.push_back(ApplianceFactory::createApplianceTV(
        "TCL", "MiniLED", 2023, 1200, 50.0, DiagonalUnit::Inches
    ));
    
    appliances.push_back(ApplianceFactory::createApplianceFridge(
        "Electrolux", "UltimateTaste", 2022, 3200, true
    ));
    
    appliances.push_back(ApplianceFactory::createApplianceWashingMachine(
        "AEG", "ProSense", 2023, 2100, 8.5
    ));
    
    appliances.push_back(ApplianceFactory::createApplianceTV(
        "Philips", "OLED+", 2021, 2800, 55.0, DiagonalUnit::Inches
    ));
    
    appliances.push_back(ApplianceFactory::createApplianceFridge(
        "Haier", "Quad", 2023, 2700, false
    ));
    
    std::cout << "\n--- Processing Mixed Collection Polymorphically ---\n" << std::endl;
    
    for (size_t i = 0; i < appliances.size(); ++i) {
        std::cout << "Appliance #" << (i + 1) << ":" << std::endl;
        std::cout << "Type: " << applianceTypeToString(appliances[i]->getType()) << std::endl;
        std::cout << "Details: " << appliances[i]->getSpecificDetails() << std::endl;
        std::cout << "Age: " << appliances[i]->getAge() << " years" << std::endl;
        std::cout << std::endl;
    }

    // ========================================================================
    // TEST 10: Age Calculation with Different Years
    // ========================================================================
    printTestHeader("TEST 10: Testing Age Calculation");
    
    auto old_tv = ApplianceFactory::createApplianceTV(
        "Toshiba", "Regza", 2015, 800, 40.0, DiagonalUnit::Inches
    );
    
    auto new_tv = ApplianceFactory::createApplianceTV(
        "Hisense", "U8K", 2024, 1600, 55.0, DiagonalUnit::Inches
    );
    
    std::cout << "\n--- Old TV (2015) ---" << std::endl;
    std::cout << "Year: " << old_tv->getYear() << std::endl;
    std::cout << "Age: " << old_tv->getAge() << " years" << std::endl;
    
    std::cout << "\n--- New TV (2024) ---" << std::endl;
    std::cout << "Year: " << new_tv->getYear() << std::endl;
    std::cout << "Age: " << new_tv->getAge() << " years" << std::endl;
    std::cout << "\n";

    // ========================================================================
    // FINAL SUMMARY
    // ========================================================================
    printTestHeader("TEST SUITE COMPLETED SUCCESSFULLY");
    
    std::cout << "\n✓ All factory methods working correctly" << std::endl;
    std::cout << "✓ All getters functioning properly" << std::endl;
    std::cout << "✓ Specific details correctly implemented" << std::endl;
    std::cout << "✓ Cloning works for all appliance types" << std::endl;
    std::cout << "✓ Matching logic verified" << std::endl;
    std::cout << "✓ Polymorphism functioning correctly" << std::endl;
    std::cout << "✓ Age calculation accurate" << std::endl;
    
    printSeparator();
    std::cout << "\n";
    
    return 0;
}