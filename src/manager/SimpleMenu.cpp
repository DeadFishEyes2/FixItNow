#include "SimpleMenu.hpp"
#include "Technician.hpp"
#include "Receptionist.hpp"
#include "Supervisor.hpp"
#include "CNPValidator.hpp"
#include <iomanip>
#include <algorithm>

SimpleMenu::SimpleMenu(ServiceManager& mgr, RepairRequestFactory& reqFactory)
    : manager(mgr), requestFactory(reqFactory), running(true) {}

void SimpleMenu::start() {
    std::cout << "========================================\n";
    std::cout << "     FixItNow Service Manager       \n";
    std::cout << "========================================\n\n";
    
    while (running) {
        handleMainMenu();
    }
}

// ============================================================================
// MAIN MENU
// ============================================================================

void SimpleMenu::displayMainMenu() {
    std::cout << "\n========== MAIN MENU ==========\n";
    std::cout << "1. Employee Management\n";
    std::cout << "2. Repair Request Management\n";
    std::cout << "3. Run Simulation\n";
    std::cout << "4. Generate Reports\n";
    std::cout << "5. Save Data\n";
    std::cout << "6. Load Data\n";
    std::cout << "7. Show Statistics\n";
    std::cout << "0. Exit\n";
    std::cout << "===============================\n";
}

void SimpleMenu::handleMainMenu() {
    clearScreen();
    displayMainMenu();
    int choice = getIntInput("Enter choice: ");
    
    switch (choice) {
        case 1:
            clearScreen();
            handleEmployeeMenu();
            break;
        case 2:
            clearScreen();
            handleRepairRequestMenu();
            break;
        case 3:
            clearScreen();
            runSimulation();
            break;
        case 4:
            clearScreen();
            generateReports();
            break;
        case 5:
            clearScreen();
            saveData();
            break;
        case 6:
            clearScreen();
            loadData();
            break;
        case 7:
            clearScreen();
            showStatistics();
            break;
        case 0:
            clearScreen();
            running = false;
            std::cout << "\nThank you for using FixItNow!\n";
            break;
        default:
            std::cout << "\nInvalid choice. Please try again.\n";
            pause();
    }
}

// ============================================================================
// EMPLOYEE MANAGEMENT
// ============================================================================

void SimpleMenu::handleEmployeeMenu() {
    clearScreen();
    std::cout << "\n=== Employee Management ===\n";
    std::cout << "1. Add Technician\n";
    std::cout << "2. Add Receptionist\n";
    std::cout << "3. Add Supervisor\n";
    std::cout << "4. List All Employees\n";
    std::cout << "5. Search Employee by CNP\n";
    std::cout << "6. Modify Employee Name\n";
    std::cout << "7. Remove Employee\n";
    std::cout << "0. Back\n";
    
    int choice = getIntInput("Enter choice: ");
    
    switch (choice) {
        case 1:
            clearScreen();
            addTechnician();
            break;
        case 2:
            clearScreen();
            addReceptionist();
            break;
        case 3:
            clearScreen();
            addSupervisor();
            break;
        case 4:
            clearScreen();
            listEmployees();
            break;
        case 5:
            clearScreen();
            searchEmployeeByCNP();
            break;
        case 6:
            clearScreen();
            modifyEmployeeName();
            break;
        case 7:
            clearScreen();
            removeEmployee();
            break;
        case 0:
            return;
        default:
            std::cout << "\nInvalid choice.\n";
            pause();
            handleEmployeeMenu();
    }
}

void SimpleMenu::addTechnician() {
    clearScreen();
    std::cout << "\n=== Add Technician ===\n";
    
    std::string firstName = getStringInput("First Name (3-30 chars): ");
    while (firstName.length() < 3 || firstName.length() > 30) {
        std::cout << "Invalid length. ";
        firstName = getStringInput("First Name (3-30 chars): ");
    }
    
    std::string lastName = getStringInput("Last Name (3-30 chars): ");
    while (lastName.length() < 3 || lastName.length() > 30) {
        std::cout << "Invalid length. ";
        lastName = getStringInput("Last Name (3-30 chars): ");
    }
    
    std::string cnp;
    do {
        cnp = getStringInput("CNP (13 digits): ");
        if (!CNPValidator::isValid(cnp)) {
            std::cout << "Error: " << CNPValidator::getValidationError(cnp) << "\n";
        }
    } while (!CNPValidator::isValid(cnp));
    
    std::cout << "\nDate of Hiring:\n";
    auto dateOfHiring = inputDate();
    
    std::cout << "\nResidence Address:\n";
    Address address = inputAddress();
    
    auto technician = EmployeeFactory::createTechnician(
        firstName, lastName, cnp, dateOfHiring, address
    );
    
    // Add skills
    std::cout << "\nAdd skills (type 'done' when finished):\n";
    std::cout << "Available types: Fridge, TV, WashingMachine\n";
    while (true) {
        std::string skillInput = getStringInput("Skill (format: Type:Brand, e.g., Fridge:Samsung): ");
        if (skillInput == "done") break;
        
        size_t colon = skillInput.find(':');
        if (colon != std::string::npos) {
            std::string typeStr = skillInput.substr(0, colon);
            std::string brand = skillInput.substr(colon + 1);
            
            try {
                ApplianceType type = stringToApplianceType(typeStr);
                Technician* tech = dynamic_cast<Technician*>(technician.get());
                tech->addSkill(type, brand);
                std::cout << "Skill added: " << typeStr << ":" << brand << "\n";
            } catch (const std::exception& e) {
                std::cout << "Invalid appliance type. Use: Fridge, TV, or WashingMachine\n";
            }
        } else {
            std::cout << "Invalid format. Use Type:Brand\n";
        }
    }
    
    int id = technician->getId();
    manager.addEmployee(std::move(technician));
    std::cout << "\nTechnician added successfully! ID: " << id << "\n";
    pause();
}

void SimpleMenu::addReceptionist() {
    clearScreen();
    std::cout << "\n=== Add Receptionist ===\n";
    
    std::string firstName = getStringInput("First Name (3-30 chars): ");
    while (firstName.length() < 3 || firstName.length() > 30) {
        std::cout << "Invalid length. ";
        firstName = getStringInput("First Name (3-30 chars): ");
    }
    
    std::string lastName = getStringInput("Last Name (3-30 chars): ");
    while (lastName.length() < 3 || lastName.length() > 30) {
        std::cout << "Invalid length. ";
        lastName = getStringInput("Last Name (3-30 chars): ");
    }
    
    std::string cnp;
    do {
        cnp = getStringInput("CNP (13 digits): ");
        if (!CNPValidator::isValid(cnp)) {
            std::cout << "Error: " << CNPValidator::getValidationError(cnp) << "\n";
        }
    } while (!CNPValidator::isValid(cnp));
    
    std::cout << "\nDate of Hiring:\n";
    auto dateOfHiring = inputDate();
    
    std::cout << "\nResidence Address:\n";
    Address address = inputAddress();
    
    auto receptionist = EmployeeFactory::createReceptionist(
        firstName, lastName, cnp, dateOfHiring, address
    );
    
    int id = receptionist->getId();
    manager.addEmployee(std::move(receptionist));
    std::cout << "\nReceptionist added successfully! ID: " << id << "\n";
    pause();
}

void SimpleMenu::addSupervisor() {
    clearScreen();
    std::cout << "\n=== Add Supervisor ===\n";
    
    std::string firstName = getStringInput("First Name (3-30 chars): ");
    while (firstName.length() < 3 || firstName.length() > 30) {
        std::cout << "Invalid length. ";
        firstName = getStringInput("First Name (3-30 chars): ");
    }
    
    std::string lastName = getStringInput("Last Name (3-30 chars): ");
    while (lastName.length() < 3 || lastName.length() > 30) {
        std::cout << "Invalid length. ";
        lastName = getStringInput("Last Name (3-30 chars): ");
    }
    
    std::string cnp;
    do {
        cnp = getStringInput("CNP (13 digits): ");
        if (!CNPValidator::isValid(cnp)) {
            std::cout << "Error: " << CNPValidator::getValidationError(cnp) << "\n";
        }
    } while (!CNPValidator::isValid(cnp));
    
    std::cout << "\nDate of Hiring:\n";
    auto dateOfHiring = inputDate();
    
    std::cout << "\nResidence Address:\n";
    Address address = inputAddress();
    
    auto supervisor = EmployeeFactory::createSupervisor(
        firstName, lastName, cnp, dateOfHiring, address
    );
    
    int id = supervisor->getId();
    manager.addEmployee(std::move(supervisor));
    std::cout << "\nSupervisor added successfully! ID: " << id << "\n";
    pause();
}

void SimpleMenu::listEmployees() {
    clearScreen();
    std::cout << "\n=== All Employees ===\n";
    
    const auto& employees = manager.getEmployees();
    
    if (employees.empty()) {
        std::cout << "No employees in the system.\n";
    } else {
        for (const auto& emp : employees) {
            std::cout << "\n------------------------\n";
            std::cout << "ID: " << emp->getId() << "\n";
            std::cout << "Name: " << emp->getFirstName() << " " << emp->getLastName() << "\n";
            std::cout << "Type: " << employeeTypeToString(emp->getType()) << "\n";
            
            // Calculate and set salary before displaying
            const_cast<Employee*>(emp.get())->setSalary();
            std::cout << "Salary: " << emp->getSalary() << " RON\n";
            
            // Show additional info for technicians
            if (emp->getType() == EmployeeType::Technician) {
                Technician* tech = dynamic_cast<Technician*>(emp.get());
                if (tech) {
                    const std::vector<Skill>& skills = tech->getSkills();
                    if (!skills.empty()) {
                        std::cout << "Skills: ";
                        for (size_t i = 0; i < skills.size(); ++i) {
                            if (i > 0) std::cout << ", ";
                            std::cout << applianceTypeToString(skills[i].type) << ":" << skills[i].brand;
                        }
                        std::cout << "\n";
                    }
                    std::cout << "Active Requests: " << tech->getActiveRequestsCount() << "\n";
                }
            }
        }
        std::cout << "------------------------\n";
    }
    
    pause();
}

void SimpleMenu::searchEmployeeByCNP() {
    clearScreen();
    std::cout << "\n=== Search Employee by CNP ===\n";
    
    std::string cnp = getStringInput("Enter CNP (13 digits): ");
    
    // Validate CNP
    if (!CNPValidator::isValid(cnp)) {
        std::cout << "\nError: " << CNPValidator::getValidationError(cnp) << "\n";
        pause();
        return;
    }
    
    Employee* emp = manager.findEmployeeByCNP(cnp);
    
    if (emp) {
        std::cout << "\n=== Employee Found ===\n";
        std::cout << "ID: " << emp->getId() << "\n";
        std::cout << "Name: " << emp->getFirstName() << " " << emp->getLastName() << "\n";
        std::cout << "CNP: " << emp->getCNP() << "\n";
        std::cout << "Type: " << employeeTypeToString(emp->getType()) << "\n";
        
        // Date of hiring
        auto date = emp->getDateOfHiring();
        std::cout << "Date of Hiring: " 
                  << static_cast<int>(date.year()) << "-"
                  << std::setfill('0') << std::setw(2) << static_cast<unsigned>(date.month()) << "-"
                  << std::setfill('0') << std::setw(2) << static_cast<unsigned>(date.day()) << "\n";
        
        // Address
        const Address& addr = emp->getResidence();
        std::cout << "City: " << addr.getCity() << "\n";
        
        // Calculate and display salary
        emp->setSalary();
        std::cout << "Current Salary: " << emp->getSalary() << " RON\n";
        
        // Type-specific information
        if (emp->getType() == EmployeeType::Technician) {
            Technician* tech = dynamic_cast<Technician*>(emp);
            if (tech) {
                const std::vector<Skill>& skills = tech->getSkills();
                if (!skills.empty()) {
                    std::cout << "\nSkills:\n";
                    for (const auto& skill : skills) {
                        std::cout << "  - " << applianceTypeToString(skill.type) 
                                  << ": " << skill.brand << "\n";
                    }
                }
                std::cout << "Active Requests: " << tech->getActiveRequestsCount() << "\n";
            }
        } else if (emp->getType() == EmployeeType::Receptionist) {
            Receptionist* rec = dynamic_cast<Receptionist*>(emp);
            if (rec) {
                std::vector<int> requests = rec->getReceivedRepairRequests();
                std::cout << "Total Received Requests: " << requests.size() << "\n";
            }
        }
    } else {
        std::cout << "\nEmployee with CNP " << cnp << " not found.\n";
    }
    
    pause();
}

void SimpleMenu::modifyEmployeeName() {
    clearScreen();
    std::cout << "\n=== Modify Employee Name ===\n";
    
    std::string cnp = getStringInput("Enter employee CNP: ");
    
    // Validate CNP
    if (!CNPValidator::isValid(cnp)) {
        std::cout << "\nError: " << CNPValidator::getValidationError(cnp) << "\n";
        pause();
        return;
    }
    
    Employee* emp = manager.findEmployeeByCNP(cnp);
    
    if (emp) {
        std::cout << "\nCurrent name: " << emp->getFirstName() << " " << emp->getLastName() << "\n";
        
        std::cout << "\nWhat would you like to change?\n";
        std::cout << "1. First Name\n";
        std::cout << "2. Last Name\n";
        std::cout << "3. Both\n";
        std::cout << "0. Cancel\n";
        
        int choice = getIntInput("Enter choice: ");
        
        switch (choice) {
            case 1: {
                std::string newFirst = getStringInput("New first name: ");
                if (newFirst.length() >= 3 && newFirst.length() <= 30) {
                    emp->changeFirstName(newFirst);
                    std::cout << "\nFirst name updated successfully!\n";
                } else {
                    std::cout << "\nError: Name must be between 3 and 30 characters.\n";
                }
                break;
            }
            case 2: {
                std::string newLast = getStringInput("New last name: ");
                if (newLast.length() >= 3 && newLast.length() <= 30) {
                    emp->changeLastName(newLast);
                    std::cout << "\nLast name updated successfully!\n";
                } else {
                    std::cout << "\nError: Name must be between 3 and 30 characters.\n";
                }
                break;
            }
            case 3: {
                std::string newFirst = getStringInput("New first name: ");
                std::string newLast = getStringInput("New last name: ");
                
                if (newFirst.length() >= 3 && newFirst.length() <= 30 &&
                    newLast.length() >= 3 && newLast.length() <= 30) {
                    emp->changeFirstName(newFirst);
                    emp->changeLastName(newLast);
                    std::cout << "\nName updated successfully!\n";
                } else {
                    std::cout << "\nError: Names must be between 3 and 30 characters.\n";
                }
                break;
            }
            case 0:
                return;
            default:
                std::cout << "\nInvalid choice.\n";
        }
        
        std::cout << "New name: " << emp->getFirstName() << " " << emp->getLastName() << "\n";
    } else {
        std::cout << "\nEmployee with CNP " << cnp << " not found.\n";
    }
    
    pause();
}

void SimpleMenu::removeEmployee() {
    clearScreen();
    std::cout << "\n=== Remove Employee ===\n";
    
    int id = getIntInput("Enter employee ID to remove: ");
    
    Employee* emp = manager.findEmployeeById(id);
    
    if (emp) {
        std::cout << "\nEmployee found:\n";
        std::cout << "ID: " << emp->getId() << "\n";
        std::cout << "Name: " << emp->getFirstName() << " " << emp->getLastName() << "\n";
        std::cout << "Type: " << employeeTypeToString(emp->getType()) << "\n";
        
        std::cout << "\nAre you sure you want to remove this employee? (y/n): ";
        std::string confirm;
        std::getline(std::cin, confirm);
        
        if (confirm == "y" || confirm == "Y") {
            manager.removeEmployeeById(id);
            std::cout << "\nEmployee removed successfully!\n";
        } else {
            std::cout << "\nRemoval cancelled.\n";
        }
    } else {
        std::cout << "\nEmployee with ID " << id << " not found.\n";
    }
    
    pause();
}

// ============================================================================
// REPAIR REQUEST MANAGEMENT
// ============================================================================

void SimpleMenu::handleRepairRequestMenu() {
    clearScreen();
    std::cout << "\n=== Repair Request Management ===\n";
    std::cout << "1. Add Repair Request\n";
    std::cout << "2. List All Repair Requests\n";
    std::cout << "3. List by Status\n";
    std::cout << "0. Back\n";
    
    int subChoice = getIntInput("Enter choice: ");
    if (subChoice == 1) {
        clearScreen();
        addRepairRequest();
    } else if (subChoice == 2) {
        clearScreen();
        listRepairRequests();
    } else if (subChoice == 3) {
        clearScreen();
        listRequestsByStatus();
    }
}

void SimpleMenu::addRepairRequest() {
    clearScreen();
    std::cout << "\n=== Add Repair Request ===\n";
    
    // Check if we have at least one receptionist
    const auto& employees = manager.getEmployees();
    bool hasReceptionist = false;
    int receptionistId = 0;
    
    for (const auto& emp : employees) {
        if (emp->getType() == EmployeeType::Receptionist) {
            hasReceptionist = true;
            receptionistId = emp->getId();
            break;
        }
    }
    
    if (!hasReceptionist) {
        std::cout << "Error: No receptionist available. Please add a receptionist first.\n";
        pause();
        return;
    }
    
    std::cout << "Receptionist ID: " << receptionistId << "\n\n";
    
    // Input appliance details
    std::cout << "Enter appliance details:\n";
    std::unique_ptr<Appliance> appliance = inputAppliance();
    
    if (!appliance) {
        std::cout << "Failed to create appliance.\n";
        pause();
        return;
    }
    
    int complexity = getIntInput("Complexity (1-5): ");
    while (complexity < 1 || complexity > 5) {
        std::cout << "Invalid complexity. Must be between 1 and 5.\n";
        complexity = getIntInput("Complexity (1-5): ");
    }
    
    // Calculate repair duration
    int age = appliance->getAge();
    int repairDuration = age * complexity;
    
    auto request = requestFactory.createRepairRequest(
        std::move(appliance),
        complexity,
        repairDuration,
        receptionistId
    );
    
    int requestId = request->getId();
    manager.addRepairRequest(std::move(request));
    
    std::cout << "\nRepair request added successfully!\n";
    std::cout << "Request ID: " << requestId << "\n";
    std::cout << "Estimated duration: " << repairDuration << " time units\n";
    
    pause();
}

void SimpleMenu::listRepairRequests() {
    clearScreen();
    std::cout << "\n=== All Repair Requests ===\n";
    
    const auto& requests = manager.getRepairRequests();
    
    if (requests.empty()) {
        std::cout << "No repair requests in the system.\n";
    } else {
        for (const auto& req : requests) {
            const Appliance& app = req->getAppliance();
            std::cout << "\n------------------------\n";
            std::cout << "Request ID: " << req->getId() << "\n";
            std::cout << "Appliance: " << applianceTypeToString(app.getType()) 
                      << " " << app.getBrand() << " " << app.getModel() << "\n";
            std::cout << "Status: " << statusToString(req->getStatus()) << "\n";
            std::cout << "Price: " << req->getPrice() << " RON\n";
            std::cout << "Complexity: " << req->getComplexity() << "\n";
            std::cout << "Total Duration: " << req->getRepairDuration() << " ticks\n";
            std::cout << "Remaining Time: " << req->getRemainingTime() << " ticks\n";
            std::cout << "Receptionist ID: " << req->getReceptionistId() << "\n";
            if (req->getTechnicianId() != 0) {
                std::cout << "Technician ID: " << req->getTechnicianId() << "\n";
            } else {
                std::cout << "Technician: Not assigned\n";
            }
        }
        std::cout << "------------------------\n";
    }
    
    pause();
}

void SimpleMenu::listRequestsByStatus() {
    clearScreen();
    std::cout << "\n=== List Requests by Status ===\n";
    std::cout << "1. Received\n";
    std::cout << "2. Pending\n";
    std::cout << "3. In Progress\n";
    std::cout << "4. Completed\n";
    std::cout << "5. Rejected\n";
    std::cout << "0. Back\n";
    
    int choice = getIntInput("Enter choice: ");
    
    Status status;
    std::string statusStr;
    
    switch (choice) {
        case 1: status = Status::Received; statusStr = "Received"; break;
        case 2: status = Status::Pending; statusStr = "Pending"; break;
        case 3: status = Status::InProgress; statusStr = "In Progress"; break;
        case 4: status = Status::Completed; statusStr = "Completed"; break;
        case 5: status = Status::Rejected; statusStr = "Rejected"; break;
        case 0: return;
        default:
            std::cout << "\nInvalid choice.\n";
            pause();
            return;
    }
    
    const auto& requests = manager.getRepairRequests();
    int count = 0;
    
    std::cout << "\n=== " << statusStr << " Requests ===\n";
    
    for (const auto& req : requests) {
        if (req->getStatus() == status) {
            count++;
            const Appliance& app = req->getAppliance();
            std::cout << "\n------------------------\n";
            std::cout << "Request ID: " << req->getId() << "\n";
            std::cout << "Appliance: " << applianceTypeToString(app.getType()) 
                      << " " << app.getBrand() << " " << app.getModel() << "\n";
            std::cout << "Price: " << req->getPrice() << " RON\n";
            std::cout << "Complexity: " << req->getComplexity() << "\n";
            
            if (status == Status::InProgress) {
                std::cout << "Remaining Time: " << req->getRemainingTime() << " ticks\n";
                std::cout << "Technician ID: " << req->getTechnicianId() << "\n";
            }
        }
    }
    
    if (count == 0) {
        std::cout << "\nNo requests with status: " << statusStr << "\n";
    } else {
        std::cout << "\n------------------------\n";
        std::cout << "Total: " << count << " request(s)\n";
    }
    
    pause();
}

// ============================================================================
// SIMULATION
// ============================================================================

void SimpleMenu::runSimulation() {
    clearScreen();
    std::cout << "\n=== Run Simulation ===\n";
    
    int ticks = getIntInput("Number of ticks to simulate: ");
    
    if (ticks <= 0) {
        std::cout << "Invalid number of ticks.\n";
        pause();
        return;
    }
    
    std::cout << "\nRunning simulation for " << ticks << " ticks...\n";
    std::cout << "========================================\n\n";
    
    for (int i = 1; i <= ticks; ++i) {
        std::cout << "[Tick " << i << "]\n";
        
        // Check received requests and auto-assign
        manager.checkReceived();
        manager.autoAssignRequests();
        
        // Perform tick (processes active requests)
        manager.tick();
        
        std::cout << "\n";
        
        // Add a small delay for readability
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "========================================\n";
    std::cout << "Simulation complete!\n";
    pause();
}

// ============================================================================
// REPORTS
// ============================================================================

void SimpleMenu::generateReports() {
    clearScreen();
    std::cout << "\n=== Generate Reports ===\n";
    std::cout << "1. Top 3 Employees by Salary\n";
    std::cout << "2. Technician with Longest Repair\n";
    std::cout << "3. Pending Requests by Appliance\n";
    std::cout << "4. Generate All Reports\n";
    std::cout << "0. Back\n";
    
    int choice = getIntInput("Enter choice: ");
    
    switch (choice) {
        case 1: {
            std::string filename = getStringInput("Output filename (default: top3_salaries.csv): ");
            if (filename.empty()) filename = "top3_salaries.csv";
            
            if (manager.generateTop3SalariesReport(filename)) {
                std::cout << "\nReport generated successfully!\n";
            } else {
                std::cout << "\nError generating report.\n";
            }
            break;
        }
        case 2: {
            std::string filename = getStringInput("Output filename (default: longest_repair.csv): ");
            if (filename.empty()) filename = "longest_repair.csv";
            
            if (manager.generateLongestRepairReport(filename)) {
                std::cout << "\nReport generated successfully!\n";
            } else {
                std::cout << "\nError generating report.\n";
            }
            break;
        }
        case 3: {
            std::string filename = getStringInput("Output filename (default: pending_requests.csv): ");
            if (filename.empty()) filename = "pending_requests.csv";
            
            if (manager.generatePendingRequestsReport(filename)) {
                std::cout << "\nReport generated successfully!\n";
            } else {
                std::cout << "\nError generating report.\n";
            }
            break;
        }
        case 4: {
            std::cout << "\nGenerating all reports...\n";
            bool success = true;
            
            success &= manager.generateTop3SalariesReport("top3_salaries.csv");
            success &= manager.generateLongestRepairReport("longest_repair.csv");
            success &= manager.generatePendingRequestsReport("pending_requests.csv");
            
            if (success) {
                std::cout << "\nAll reports generated successfully!\n";
            } else {
                std::cout << "\nSome reports failed to generate.\n";
            }
            break;
        }
        case 0:
            return;
        default:
            std::cout << "\nInvalid choice.\n";
    }
    
    pause();
}

// ============================================================================
// DATA MANAGEMENT
// ============================================================================

void SimpleMenu::saveData() {
    clearScreen();
    std::cout << "\n=== Save Data ===\n";
    
    std::string empFile = getStringInput("Employees file (default: Employees.csv): ");
    if (empFile.empty()) empFile = "Employees.csv";
    
    std::string reqFile = getStringInput("Repair requests file (default: RepairRequests.csv): ");
    if (reqFile.empty()) reqFile = "RepairRequests.csv";
    
    if (manager.saveToCSV(empFile, reqFile)) {
        std::cout << "\nData saved successfully!\n";
    } else {
        std::cout << "\nError: Failed to save data.\n";
    }
    
    pause();
}

void SimpleMenu::loadData() {
    clearScreen();
    std::cout << "\n=== Load Data ===\n";
    
    std::string empFile = getStringInput("Employees file (default: Employees.csv): ");
    if (empFile.empty()) empFile = "Employees.csv";
    
    std::string reqFile = getStringInput("Repair requests file (default: RepairRequests.csv): ");
    if (reqFile.empty()) reqFile = "RepairRequests.csv";
    
    if (manager.loadFromCSV(requestFactory, empFile, reqFile)) {
        std::cout << "\nData loaded successfully!\n";
    } else {
        std::cout << "\nError: Failed to load data.\n";
    }
    
    pause();
}

// ============================================================================
// STATISTICS
// ============================================================================

void SimpleMenu::showStatistics() {
    clearScreen();
    std::cout << "\n=== Service Statistics ===\n";
    
    const auto& employees = manager.getEmployees();
    const auto& requests = manager.getRepairRequests();
    
    // Count employees by type
    int techCount = 0, recepCount = 0, supervCount = 0;
    for (const auto& emp : employees) {
        switch (emp->getType()) {
            case EmployeeType::Technician: techCount++; break;
            case EmployeeType::Receptionist: recepCount++; break;
            case EmployeeType::Supervisor: supervCount++; break;
        }
    }
    
    // Count requests by status
    int receivedCount = 0, pendingCount = 0, inProgressCount = 0, completedCount = 0, rejectedCount = 0;
    for (const auto& req : requests) {
        switch (req->getStatus()) {
            case Status::Received: receivedCount++; break;
            case Status::Pending: pendingCount++; break;
            case Status::InProgress: inProgressCount++; break;
            case Status::Completed: completedCount++; break;
            case Status::Rejected: rejectedCount++; break;
        }
    }
    
    std::cout << "\n--- Employees ---\n";
    std::cout << "Total: " << employees.size() << "\n";
    std::cout << "  Technicians: " << techCount << "\n";
    std::cout << "  Receptionists: " << recepCount << "\n";
    std::cout << "  Supervisors: " << supervCount << "\n";
    
    std::cout << "\n--- Repair Requests ---\n";
    std::cout << "Total: " << requests.size() << "\n";
    std::cout << "  Received: " << receivedCount << "\n";
    std::cout << "  Pending: " << pendingCount << "\n";
    std::cout << "  In Progress: " << inProgressCount << "\n";
    std::cout << "  Completed: " << completedCount << "\n";
    std::cout << "  Rejected: " << rejectedCount << "\n";
    
    // Calculate total revenue from completed requests
    int totalRevenue = 0;
    for (const auto& req : requests) {
        if (req->getStatus() == Status::Completed) {
            totalRevenue += req->getPrice();
        }
    }
    
    std::cout << "\n--- Financial ---\n";
    std::cout << "Total Revenue (Completed): " << totalRevenue << " RON\n";
    
    // Calculate total salary expenses (make sure salaries are calculated)
    int totalSalaries = 0;
    for (const auto& emp : employees) {
        const_cast<Employee*>(emp.get())->setSalary();
        totalSalaries += emp->getSalary();
    }
    std::cout << "Total Monthly Salaries: " << totalSalaries << " RON\n";
    
    // Show active work
    std::cout << "\n--- Active Work ---\n";
    int totalRemainingTicks = 0;
    for (const auto& req : requests) {
        if (req->getStatus() == Status::InProgress) {
            totalRemainingTicks += req->getRemainingTime();
        }
    }
    std::cout << "Total Remaining Work: " << totalRemainingTicks << " ticks\n";
    
    pause();
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

Address SimpleMenu::inputAddress() {
    std::string country = getStringInput("Country: ");
    std::string county = getStringInput("County: ");
    std::string city = getStringInput("City: ");
    std::string street = getStringInput("Street: ");
    std::string number = getStringInput("Number: ");
    
    std::cout << "Address type (1: House, 2: Apartment): ";
    int type = getIntInput("");
    
    if (type == 2) {
        std::string building = getStringInput("Building: ");
        std::string floor = getStringInput("Floor: ");
        std::string aptNumber = getStringInput("Apartment Number: ");
        return *AddressFactory::createAddressApartment(
            country, county, city, street, number, building, floor, aptNumber
        );
    } else {
        return *AddressFactory::createAddressHouse(
            country, county, city, street, number
        );
    }
}

std::unique_ptr<Appliance> SimpleMenu::inputAppliance() {
    std::cout << "Appliance Type:\n";
    std::cout << "1. Fridge\n";
    std::cout << "2. TV\n";
    std::cout << "3. Washing Machine\n";
    int type = getIntInput("Choice: ");
    
    std::string brand = getStringInput("Brand: ");
    std::string model = getStringInput("Model: ");
    int year = getIntInput("Year of Manufacture: ");
    int price = getIntInput("Catalog Price (RON): ");
    
    switch (type) {
        case 1: {
            std::cout << "Has Freezer? (1: Yes, 0: No): ";
            int hasFreezer = getIntInput("");
            return ApplianceFactory::createApplianceFridge(
                brand, model, year, price, hasFreezer == 1
            );
        }
        case 2: {
            double diagonal = getDoubleInput("Diagonal Value: ");
            std::cout << "Unit (1: Centimeters, 2: Inches): ";
            int unit = getIntInput("");
            DiagonalUnit diagUnit = (unit == 1) ? DiagonalUnit::Centimeters : DiagonalUnit::Inches;
            return ApplianceFactory::createApplianceTV(
                brand, model, year, price, diagonal, diagUnit
            );
        }
        case 3: {
            double capacity = getDoubleInput("Capacity (kg): ");
            return ApplianceFactory::createApplianceWashingMachine(
                brand, model, year, price, capacity
            );
        }
        default:
            std::cout << "Invalid appliance type.\n";
            return nullptr;
    }
}

std::chrono::year_month_day SimpleMenu::inputDate() {
    int year = getIntInput("Year (e.g., 2020): ");
    int month = getIntInput("Month (1-12): ");
    int day = getIntInput("Day (1-31): ");
    
    return std::chrono::year_month_day{
        std::chrono::year{year},
        std::chrono::month{static_cast<unsigned>(month)},
        std::chrono::day{static_cast<unsigned>(day)}
    };
}

void SimpleMenu::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void SimpleMenu::pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int SimpleMenu::getIntInput(const std::string& prompt) {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

std::string SimpleMenu::getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

double SimpleMenu::getDoubleInput(const std::string& prompt) {
    double value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

std::string SimpleMenu::statusToString(Status status) {
    switch (status) {
        case Status::Received: return "Received";
        case Status::Pending: return "Pending";
        case Status::InProgress: return "In Progress";
        case Status::Completed: return "Completed";
        case Status::Rejected: return "Rejected";
        default: return "Unknown";
    }
}

std::string SimpleMenu::employeeTypeToString(EmployeeType type) {
    switch (type) {
        case EmployeeType::Technician: return "Technician";
        case EmployeeType::Receptionist: return "Receptionist";
        case EmployeeType::Supervisor: return "Supervisor";
        default: return "Unknown";
    }
}