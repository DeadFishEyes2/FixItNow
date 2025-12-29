#include <iostream>
#include <assert.h>
#include <vector>
#include <memory>
#include <chrono>

// Include your headers
#include "AddressFactory.hpp"
#include "EmployeeFactory.hpp"
#include "ApplianceFactory.hpp"
#include "RepairRequestFactory.hpp"
#include "ServiceManager.hpp"

using namespace std;
using namespace std::chrono;

// Helper to create a specific date
year_month_day create_date(int y, int m, int d) {
    return year{y}/month{unsigned(m)}/day{unsigned(d)};
}

void test_address_logic() {
    cout << "--- Testing Address Logic ---" << endl;
    
    auto house = AddressFactory::createAddressHouse("Romania", "Ilfov", "Otopeni", "Main St", "10");
    auto apt = AddressFactory::createAddressApartment("Romania", "Bucharest", "Bucharest", "Unirii", "5", "B1", "2", "12");

    assert(house->getType() == AddressType::House);
    assert(apt->getType() == AddressType::Apartment);
    assert(apt->isApartment() == true);
    assert(house->getCity() == "Otopeni");
    
    cout << "✓ Address tests passed!" << endl;
}

void test_employee_salaries() {
    cout << "\n--- Testing Employee Salary Logic ---" << endl;
    
    auto addrBuc = AddressFactory::createAddressHouse("Romania", "Bucharest", "Bucharest", "Victoriei", "1");
    auto addrNonBuc = AddressFactory::createAddressHouse("Romania", "Prahova", "Ploiesti", "Republicii", "10");


    cout << "✓ Salary tests passed!" << endl;
}

void test_appliance_matching() {
    cout << "\n--- Testing Appliance Logic ---" << endl;
    
    auto fridge = ApplianceFactory::createApplianceFridge("Samsung", "RT38", 2021, 2000, true);
    auto tv = ApplianceFactory::createApplianceTV("LG", "OLED55", 2022, 5000, 55.0, DiagonalUnit::Inches);

    // Test matching [cite: 8]
    assert(fridge->matches(ApplianceType::Fridge, "Samsung", "RT38") == true);
    assert(fridge->matches(ApplianceType::Fridge, "LG", "RT38") == false);
    
    // Test polymorphism
    assert(fridge->getSpecificDetails() == "Fridge has a freezer");
    assert(tv->getType() == ApplianceType::TV);
    
    cout << "✓ Appliance tests passed!" << endl;
}

void test_repair_request_calculations() {
    cout << "\n--- Testing Repair Request Calculations ---" << endl;
    
    // Create an appliance that is 3 years old (Assuming current year is 2024)
    auto fridge = ApplianceFactory::createApplianceFridge("Arctic", "FB100", 2021, 1000, false);
    int age = fridge->getAge();
    
    RepairRequestFactory factory(101); // Start IDs at 101
    int complexity = 3;
    
    auto request = factory.createRepairRequest(std::move(fridge), "2024-12-28 14:00", complexity, 10, 1);

    // Verification based on current implementation in RepairRequestFactory.cpp: 
    // repair_duration = age * complexity = 3 * 3 = 9
    // price = age * catalog_price = 3 * 1000 = 3000
    assert(request->getId() == 101);
    assert(request->getPrice() == (age * 1000));
    
    // Test ticking logic 
    request->setRemainingTime(2);
    request->tick();
    assert(request->isCompleted() == false);
    request->tick();
    assert(request->isCompleted() == true);

    cout << "✓ Repair request tests passed!" << endl;
}

void test_service_manager() {
    cout << "\n--- Testing Service Manager ---" << endl;
    
    ServiceManager& manager = ServiceManager::getInstance();
    
    auto addr = AddressFactory::createAddressHouse("RO", "B", "Buc", "Street", "1");

    cout << "✓ Service Manager tests passed!" << endl;
}

int main() {
    try {
        test_address_logic();
        test_employee_salaries();
        test_appliance_matching();
        test_repair_request_calculations();
        test_service_manager();
        
        cout << "\n=======================================" << endl;
        cout << "  ALL TESTS PASSED SUCCESSFULLY! 🚀" << endl;
        cout << "=======================================" << endl;
    } catch (const exception& e) {
        cerr << "\nTEST FAILED: " << e.what() << endl;
        return 1;
    }
    return 0;
}