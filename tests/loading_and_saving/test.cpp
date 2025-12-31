// Example of how to use the CSV save/load functionality

#include "ServiceManager.hpp"
#include "RepairRequestFactory.hpp"
#include "EmployeeFactory.hpp"

int main() {
    // Get ServiceManager instance (Singleton)
    ServiceManager& manager = ServiceManager::getInstance();
    
    // Create RepairRequestFactory
    RepairRequestFactory requestFactory(1); // Start with ID 1
    
    // ========================================================================
    // SCENARIO 1: Starting fresh and saving data
    // ========================================================================
    
    // Add some employees
    auto addr1 = AddressFactory::createAddressHouse(
        "Romania", "Bucharest", "Bucharest", "Main St", "10"
    );
    auto tech1 = EmployeeFactory::createTechnician(
        "John", "Doe", "1234567890123",
        std::chrono::year_month_day{std::chrono::year{2020}, std::chrono::January, std::chrono::day{15}},
        *addr1
    );
    
    Technician* tech_ptr = dynamic_cast<Technician*>(tech1.get());
    tech_ptr->addSkill(ApplianceType::Fridge, "Samsung");
    tech_ptr->addSkill(ApplianceType::TV, "LG");
    
    manager.addEmployee(std::move(tech1));
    
    // Add some repair requests
    auto fridge = ApplianceFactory::createApplianceFridge(
        "Samsung", "Frost200", 2018, 1500, true
    );
    auto request1 = requestFactory.createRepairRequest(
        std::move(fridge), 3, 18, 1
    );
    manager.addRepairRequest(std::move(request1));
    
    // Save all data to CSV files
    if (manager.saveToCSV("Employees.csv", "RepairRequests.csv")) {
        std::cout << "All data saved successfully!\n";
    }
    
    // ========================================================================
    // SCENARIO 2: Loading existing data on program startup
    // ========================================================================
    
    // Clear current data (for demonstration)
    // In real usage, you'd do this on a fresh program start
    
    // Create fresh instances
    ServiceManager& newManager = ServiceManager::getInstance();
    RepairRequestFactory newRequestFactory(1);
    
    // Load data from CSV files
    if (newManager.loadFromCSV(newRequestFactory, "Employees.csv", "RepairRequests.csv")) {
        std::cout << "All data loaded successfully!\n";
        
        // Verify loaded data
        const auto& employees = newManager.getEmployees();
        const auto& requests = newManager.getRepairRequests();
        
        std::cout << "Loaded " << employees.size() << " employees\n";
        std::cout << "Loaded " << requests.size() << " repair requests\n";
    }
    
    // ========================================================================
    // SCENARIO 3: Periodic auto-save (e.g., every 5 minutes or on shutdown)
    // ========================================================================
    
    // Add this to your main loop or before program exit
    auto saveData = [&]() {
        manager.saveToCSV("Employees.csv", "RepairRequests.csv");
    };
    
    // Call saveData() periodically or on SIGINT/exit
    
    // ========================================================================
    // SCENARIO 4: Integration with existing ServiceManagerTUI
    // ========================================================================
    
    // In ServiceManagerTUI, add menu options for save/load
    /*
    void ServiceManagerTUI::handleSaveData() {
        if (manager.saveToCSV("Employees.csv", "RepairRequests.csv")) {
            showMessage("Data saved successfully!");
        } else {
            showMessage("Error: Failed to save data");
        }
    }
    
    void ServiceManagerTUI::handleLoadData() {
        if (manager.loadFromCSV(requestFactory, "Employees.csv", "RepairRequests.csv")) {
            showMessage("Data loaded successfully!");
        } else {
            showMessage("Error: Failed to load data");
        }
    }
    */
    
    return 0;
}

// ============================================================================
// IMPORTANT NOTES
// ============================================================================

/*
1. Always create RepairRequestFactory BEFORE loading data, as it's needed for
   loading repair requests.

2. The factories automatically update their next_id counters when loading data
   with existing IDs, ensuring no ID conflicts.

3. Load employees BEFORE repair requests if there are dependencies (like
   receptionist_id references).

4. The CSV files are human-readable and can be edited manually if needed,
   but be careful with the format.

5. Error handling: Both save and load functions continue on errors and report
   which lines failed, so partial data can still be recovered.

6. For production use, consider:
   - Automatic backup before overwriting existing CSV files
   - Compression for large datasets
   - Encryption if data is sensitive
   - Database instead of CSV for better performance with large datasets
*/