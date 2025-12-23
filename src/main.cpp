#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

#include "ApplianceFactory.hpp"
#include "AddressFactory.hpp"
#include "RepairRequestFactory.hpp"

// Utility to print headers for test sections
void printTestHeader(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << " TEST CASE: " << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

int main() {
    // 1. Initialize Factories
    RepairRequestFactory reqFactory(100); // Start IDs from 100
    
    // 2. Create various Appliances using the ApplianceFactory
    auto tv = ApplianceFactory::createApplianceTV("Sony", "Bravia XR", 2020, 1200, 55.0, DiagonalUnit::Inches);
    auto fridge = ApplianceFactory::createApplianceFridge("Samsung", "Family Hub", 2018, 2500, true);
    auto washer = ApplianceFactory::createApplianceWashingMachine("LG", "ThinQ", 2022, 800, 9.0);
    
    // 3. Create Addresses using the AddressFactory
    auto homeAddr = AddressFactory::createAddressHouse("Romania", "Bucuresti", "Sector 1", "Str. Victoriei", "10");
    auto aptAddr = AddressFactory::createAddressApartment("Romania", "Cluj", "Cluj-Napoca", "Str. Eroilor", "45", "Bloc A", "Etaj 3", "12");

    // ========================================================================
    printTestHeader("1. Repair Request Creation & Auto-Calculations");
    // ========================================================================
    
    // Create requests with different complexities
    // Logic: price = Age * CatalogPrice; duration = Age * complexity
    std::vector<std::unique_ptr<RepairRequest>> requests;
    
    requests.push_back(reqFactory.createRepairRequest(std::move(tv), "2023-10-27 10:00", 3, 5, 1));
    requests.push_back(reqFactory.createRepairRequest(std::move(fridge), "2023-10-27 11:30", 5, 8, 1));
    requests.push_back(reqFactory.createRepairRequest(std::move(washer), "2023-10-27 12:15", 2, 3, 2));

    for (const auto& req : requests) {
        std::cout << "Request ID: " << req->getId() << "\n";
        std::cout << "Appliance: " << req->getAppliance().getBrand() << " " << req->getAppliance().getModel() << "\n";
        std::cout << "Calculated Price: " << req->getPrice() << " RON\n";
        std::cout << "Receptionist ID: " << req->getReceptionistId() << "\n";
        std::cout << "-----------------------------------\n";
    }

    // ========================================================================
    printTestHeader("2. Comparing Requests (Business Logic)");
    // ========================================================================
    
    // Find the most expensive repair
    auto mostExpensive = std::max_element(requests.begin(), requests.end(), 
        [](const std::unique_ptr<RepairRequest>& a, const std::unique_ptr<RepairRequest>& b) {
            return a->getPrice() < b->getPrice();
        });

    if (mostExpensive != requests.end()) {
        std::cout << "Most Expensive Request is ID " << (*mostExpensive)->getId() 
                  << " costing " << (*mostExpensive)->getPrice() << " RON.\n";
    }

    // Filter requests for specific brands (e.g., LG)
    std::cout << "Searching for LG Appliances in requests...\n";
    for (const auto& req : requests) {
        if (req->getAppliance().getBrand() == "LG") {
            std::cout << "Found: Request " << req->getId() << " for brand " << req->getAppliance().getBrand() << "\n";
        }
    }

    // ========================================================================
    printTestHeader("3. Technician Assignment & Time Ticking Simulation");
    // ========================================================================
    
    // Assign a technician to the first request
    requests[0]->setTechnicianId(501);
    std::cout << "Request " << requests[0]->getId() << " assigned to Technician " << requests[0]->getTechnicianId() << "\n";
    
    // Simulation of repair progress
    std::cout << "Simulating 3 hours of work on the first request...\n";
    requests[0]->setRemainingTime(10); // Manually set for test
    
    for (int i = 0; i < 3; ++i) {
        requests[0]->tick(); //
        std::cout << "Tick... Work in progress.\n";
    }
    
    // ========================================================================
    printTestHeader("4. Polymorphism & Specific Details");
    // ========================================================================
    
    // Accessing specific details through the base Appliance pointer held in RepairRequest
    for (const auto& req : requests) {
        const Appliance& app = req->getAppliance();
        std::cout << "Request #" << req->getId() << " Details: " << app.getSpecificDetails() << "\n";
    }

    // ========================================================================
    printTestHeader("5. Address Logging");
    // ========================================================================
    
    std::cout << "Logging House Address:\n";
    homeAddr->log(); //
    
    std::cout << "\nLogging Apartment Address:\n";
    aptAddr->log(); //

    std::cout << "\n" << std::string(60, '*') << "\n";
    std::cout << " ALL TESTS COMPLETED SUCCESSFULLY\n";
    std::cout << std::string(60, '*') << "\n";

    return 0;
}