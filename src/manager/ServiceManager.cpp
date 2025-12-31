#include "ServiceManager.hpp"

void ServiceManager::addEmployee(std::unique_ptr<Employee> employee) {
    employees.push_back(std::move(employee));    
};

void ServiceManager::addRepairRequest(std::unique_ptr<RepairRequest> repair_request) {
    //repair_request->setStatus(Status::Pending);
    repair_requests.push_back(std::move(repair_request));
}

RepairRequest* ServiceManager::findRepairRequestById(int id) const {
    for (auto& request : repair_requests) {
        if (request->getId() == id) {
            return request.get();
        }
    }
    return nullptr; // Not found
}

void ServiceManager::removeEmployeeById(int id) {
    std::erase_if(employees, [id](const std::unique_ptr<Employee>& emp) {
        return emp->getId() == id;
    });
}

Employee* ServiceManager::findEmployeeById(int id){
    for (auto& employee : employees) {
        if (employee->getId() == id) {
            return employee.get();
        }
    }
    return nullptr; // Not found
}

Employee* ServiceManager::findEmployeeByCNP(std::string CNP){
    for (auto& employee : employees) {
        if (employee->getCNP() == CNP) {
            return employee.get();
        }
    }
    return nullptr; // Not found
}

std::vector<int> ServiceManager::filter(
        Status status,
        std::chrono::system_clock::time_point start,
        std::chrono::system_clock::time_point end,
        int technician_id,
        int receptionist_id)
{
    std::vector<int> filtered_ids;

    // Iterate through all stored repair requests
    for (const auto& request : repair_requests) {
        
        // Filter by Status
        if (request->getStatus() != status) {
            continue;
        }

        // Filter by Technician ID
        // If technician_id is 0, we treat it as "any technician"
        if (technician_id != 0 && request->getTechnicianId() != technician_id) {
            continue;
        }

        // Filter by Receptionist ID
        // If receptionist_id is 0, we treat it as "any receptionist"
        if (receptionist_id != 0 && request->getReceptionistId() != receptionist_id) {
            continue;
        }

        // Checks if the request's timestamp falls within the [start, end] range
        auto request_time = request->getTimestamp();
        if (request_time < start || request_time > end) {
            continue;
        }

        // If the request passes all active filters, add its ID to the results
        filtered_ids.push_back(request->getId());
    }

    return filtered_ids;
}

bool ServiceManager::canRepair(int repair_request_id) const {
    const RepairRequest* request = findRepairRequestById(repair_request_id);
    if (!request) return false;

    const Appliance& appliance = request->getAppliance();

    for (const auto& emp : employees) {
        // Check if the employee is a Technician
        Technician* tech = dynamic_cast<Technician*>(emp.get());
        if (tech && tech->canRepair(appliance)) {
            return true; // Found at least one technician who can fix it
        }
    }
    return false;
}

void ServiceManager::checkReceived(){
    
    std::vector<int> recieved_repair_request_ids = filter(Status::Received);
    for (auto recieved_repair_request_id : recieved_repair_request_ids){
        if (canRepair(recieved_repair_request_id)){
            findRepairRequestById(recieved_repair_request_id)->setStatus(Status::Pending);
        } else {
            findRepairRequestById(recieved_repair_request_id)->setStatus(Status::Rejected);
        }
    }
}

void ServiceManager::autoAssignRequests() {

    checkReceived(); 

    //Get all requests that are ready to be assigned
    std::vector<int> pending_ids = filter(Status::Pending);

    for (int req_id : pending_ids) {
        RepairRequest* request = findRepairRequestById(req_id);
        if (!request) continue;

        const Appliance& appliance = request->getAppliance();

        Technician* best_tech = nullptr;
        int min_load = 4; // Since the limit is 3, 4 acts as "infinity"

        // Search for the best technician for this specific appliance
        for (auto& emp : employees) {
            Technician* tech = dynamic_cast<Technician*>(emp.get());
            
            // Check if they are a technician and have the matching skill
            if (tech && tech->canRepair(appliance)) {
                int current_load = tech->getActiveRequestsCount();
                
                // Only consider if they have space (< 3) 
                // and if they have fewer requests than our previous best candidate
                if (current_load < 3 && current_load < min_load) {
                    min_load = current_load;
                    best_tech = tech;
                }
            }
        }

        // If a candidate was found, perform the assignment
        if (best_tech) {
            request->setStatus(Status::InProgress);
            request->setTechnicianId(best_tech->getId()); 
            best_tech->addActiveRequest(req_id); 
            
            std::cout << "Assigned Request #" << req_id << " to Tech " 
                      << best_tech->getId() << " (Current Load: " << min_load + 1 << ")\n";
        }
    }
}

void ServiceManager::tick() {
    // Try to assign any Received/Pending requests to available technicians
    autoAssignRequests();

    // Iterate through all employees and "tick" the technicians
    for (auto& emp : employees) {
        // Since employees are stored as unique_ptr<Employee>, 
        // we use dynamic_cast to identify Technicians
        Technician* tech = dynamic_cast<Technician*>(emp.get());
        
        if (tech) {
            // This calls Technician::tick(), which decrements remaining time 
            // for their active requests
            tech->tick();
        }
    }
}

const std::vector<std::unique_ptr<Employee>>& ServiceManager::getEmployees() const { return employees; }
const std::vector<std::unique_ptr<RepairRequest>>& ServiceManager::getRepairRequests() const { return repair_requests; }

// ============================================================================
// CSV HELPER FUNCTIONS
// ============================================================================

std::string ServiceManager::escapeCSV(const std::string& field) {
    if (field.find(',') != std::string::npos || 
        field.find('"') != std::string::npos || 
        field.find('\n') != std::string::npos) {
        std::string escaped = "\"";
        for (char c : field) {
            if (c == '"') escaped += "\"\"";
            else escaped += c;
        }
        escaped += "\"";
        return escaped;
    }
    return field;
}

std::vector<std::string> ServiceManager::parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool in_quotes = false;
    
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        
        if (c == '"') {
            if (in_quotes && i + 1 < line.length() && line[i + 1] == '"') {
                field += '"';
                ++i;
            } else {
                in_quotes = !in_quotes;
            }
        } else if (c == ',' && !in_quotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(field);
    return fields;
}

// ============================================================================
// SAVE EMPLOYEES TO CSV
// ============================================================================

bool ServiceManager::saveEmployeesToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for writing.\n";
        return false;
    }
    
    // Write header
    file << "ID,Type,FirstName,LastName,CNP,DateOfHiring,Country,County,City,Street,Number,AddressType,";
    file << "AptBuilding,AptFloor,AptNumber,Skills,ReceivedRequests\n";
    
    for (const auto& emp : employees) {
        file << emp->getId() << ",";
        
        // Employee Type
        EmployeeType type = emp->getType();
        if (type == EmployeeType::Receptionist) file << "Receptionist,";
        else if (type == EmployeeType::Technician) file << "Technician,";
        else if (type == EmployeeType::Supervisor) file << "Supervisor,";
        
        file << escapeCSV(emp->getFirstName()) << ",";
        file << escapeCSV(emp->getLastName()) << ",";
        file << emp->getCNP() << ",";
        
        // Date of hiring
        auto date = emp->getDateOfHiring();
        file << static_cast<int>(date.year()) << "-"
             << std::setfill('0') << std::setw(2) << static_cast<unsigned>(date.month()) << "-"
             << std::setfill('0') << std::setw(2) << static_cast<unsigned>(date.day()) << ",";
        
        // Address information
        const Address& addr = emp->getResidence();
        file << escapeCSV(addr.getCountry()) << ",";
        file << escapeCSV(addr.getCounty()) << ",";
        file << escapeCSV(addr.getCity()) << ",";
        file << escapeCSV(addr.getStreet()) << ",";
        file << escapeCSV(addr.getNumber()) << ",";
        
        // Address type
        if (addr.isApartment()) {
            file << "Apartment,";
            const ApartmentDetails* apt = addr.getApartmentDetails();
            if (apt) {
                file << escapeCSV(apt->building) << ",";
                file << escapeCSV(apt->floor) << ",";
                file << escapeCSV(apt->apartment_number) << ",";
            } else {
                file << ",,,";
            }
        } else {
            file << "House,,,,";
        }
        
        // Type-specific data
        if (type == EmployeeType::Technician) {
            Technician* tech = dynamic_cast<Technician*>(emp.get());
            const std::vector<Skill>& skills = tech->getSkills();
            std::string skills_str;
            for (size_t i = 0; i < skills.size(); ++i) {
                if (i > 0) skills_str += ";";
                skills_str += applianceTypeToString(skills[i].type) + ":" + skills[i].brand;
            }
            file << escapeCSV(skills_str) << ",";
        } else {
            file << ",";
        }
        
        if (type == EmployeeType::Receptionist) {
            Receptionist* rec = dynamic_cast<Receptionist*>(emp.get());
            std::vector<int> requests = rec->getReceivedRepairRequests();
            std::string requests_str;
            for (size_t i = 0; i < requests.size(); ++i) {
                if (i > 0) requests_str += ";";
                requests_str += std::to_string(requests[i]);
            }
            file << escapeCSV(requests_str);
        } else {
            file << "";
        }
        
        file << "\n";
    }
    
    file.close();
    return true;
}

// ============================================================================
// SAVE REPAIR REQUESTS TO CSV
// ============================================================================

bool ServiceManager::saveRepairRequestsToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for writing.\n";
        return false;
    }
    
    // Write header
    file << "ID,ApplianceType,Brand,Model,Year,CatalogPrice,";
    file << "HasFreezer,DiagonalValue,DiagonalUnit,Capacity,";
    file << "Timestamp,Complexity,RepairDuration,RemainingTime,Price,Status,";
    file << "ReceptionistID,TechnicianID\n";
    
    for (const auto& req : repair_requests) {
        file << req->getId() << ",";
        
        const Appliance& app = req->getAppliance();
        
        // Appliance type
        ApplianceType app_type = app.getType();
        file << applianceTypeToString(app_type) << ",";
        file << escapeCSV(app.getBrand()) << ",";
        file << escapeCSV(app.getModel()) << ",";
        file << app.getYear() << ",";
        file << app.getCatalogPrice() << ",";
        
        // Type-specific appliance data
        if (app_type == ApplianceType::Fridge) {
            const Fridge* fridge = dynamic_cast<const Fridge*>(&app);
            file << (fridge->hasFreezer() ? "1" : "0") << ",,,";
        } else if (app_type == ApplianceType::TV) {
            const TV* tv = dynamic_cast<const TV*>(&app);
            const Diagonal& diag = tv->getDiagonal();
            file << "," << diag.value << ",";
            file << (diag.unit == DiagonalUnit::Centimeters ? "cm" : "inch") << ",";
        } else if (app_type == ApplianceType::WashingMachine) {
            const WashingMachine* wm = dynamic_cast<const WashingMachine*>(&app);
            file << ",,," << wm->getCapacity();
        }
        
        // Timestamp (convert to time_t for easier storage)
        auto timestamp = req->getTimestamp();
        auto time_t_val = std::chrono::system_clock::to_time_t(timestamp);
        file << "," << time_t_val << ",";
        
        // Request details
        file << req->getComplexity() << ",";
        file << req->getRepairDuration() << ",";
        file << req->getRemainingTime() << ",";
        file << req->getPrice() << ",";
        
        // Status
        Status status = req->getStatus();
        if (status == Status::Received) file << "Received,";
        else if (status == Status::Pending) file << "Pending,";
        else if (status == Status::InProgress) file << "InProgress,";
        else if (status == Status::Completed) file << "Completed,";
        else if (status == Status::Rejected) file << "Rejected,";
        
        file << req->getReceptionistId() << ",";
        file << req->getTechnicianId() << "\n";
    }
    
    file.close();
    return true;
}

// ============================================================================
// LOAD EMPLOYEES FROM CSV
// ============================================================================

bool ServiceManager::loadEmployeesFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for reading.\n";
        return false;
    }
    
    std::string line;
    std::getline(file, line); // Skip header
    
    int line_num = 1;
    while (std::getline(file, line)) {
        ++line_num;
        if (line.empty()) continue;
        
        try {
            std::vector<std::string> fields = parseCSVLine(line);
            
            if (fields.size() < 16) {
                std::cerr << "Error: Invalid employee data on line " << line_num << "\n";
                continue;
            }
            
            int id = std::stoi(fields[0]);
            std::string type_str = fields[1];
            std::string first_name = fields[2];
            std::string last_name = fields[3];
            std::string cnp = fields[4];
            
            // Parse date (format: YYYY-MM-DD)
            std::string date_str = fields[5];
            int year, month, day;
            char dash1, dash2;
            std::istringstream date_stream(date_str);
            date_stream >> year >> dash1 >> month >> dash2 >> day;
            std::chrono::year_month_day date{
                std::chrono::year{year}, 
                std::chrono::month{static_cast<unsigned>(month)}, 
                std::chrono::day{static_cast<unsigned>(day)}
            };
            
            std::string country = fields[6];
            std::string county = fields[7];
            std::string city = fields[8];
            std::string street = fields[9];
            std::string number = fields[10];
            std::string address_type = fields[11];
            
            // Create address
            std::unique_ptr<Address> addr;
            if (address_type == "Apartment") {
                addr = AddressFactory::createAddressApartment(
                    country, county, city, street, number,
                    fields[12], fields[13], fields[14]
                );
            } else {
                addr = AddressFactory::createAddressHouse(
                    country, county, city, street, number
                );
            }
            
            // Create employee based on type using the new WithID factory methods
            std::unique_ptr<Employee> emp;
            if (type_str == "Receptionist") {
                emp = EmployeeFactory::createReceptionistWithID(
                    id, first_name, last_name, cnp, date, *addr
                );
                
                // Load received requests
                if (!fields[16].empty()) {
                    Receptionist* rec = dynamic_cast<Receptionist*>(emp.get());
                    std::stringstream ss(fields[16]);
                    std::string req_id_str;
                    std::vector<int> requests;
                    while (std::getline(ss, req_id_str, ';')) {
                        if (!req_id_str.empty()) {
                            requests.push_back(std::stoi(req_id_str));
                        }
                    }
                    rec->setReceivedRepairRequests(requests);
                }
            } else if (type_str == "Technician") {
                emp = EmployeeFactory::createTechnicianWithID(
                    id, first_name, last_name, cnp, date, *addr
                );
                
                // Load skills
                if (!fields[15].empty()) {
                    Technician* tech = dynamic_cast<Technician*>(emp.get());
                    std::stringstream ss(fields[15]);
                    std::string skill_str;
                    while (std::getline(ss, skill_str, ';')) {
                        size_t colon = skill_str.find(':');
                        if (colon != std::string::npos) {
                            std::string type_part = skill_str.substr(0, colon);
                            std::string brand = skill_str.substr(colon + 1);
                            ApplianceType app_type = stringToApplianceType(type_part);
                            tech->addSkill(app_type, brand);
                        }
                    }
                }
            } else if (type_str == "Supervisor") {
                emp = EmployeeFactory::createSupervisorWithID(
                    id, first_name, last_name, cnp, date, *addr
                );
            }
            
            if (emp) {
                addEmployee(std::move(emp));
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Error parsing employee on line " << line_num << ": " << e.what() << "\n";
        }
    }
    
    file.close();
    return true;
}

// ============================================================================
// LOAD REPAIR REQUESTS FROM CSV
// ============================================================================

bool ServiceManager::loadRepairRequestsFromCSV(RepairRequestFactory& requestFactory, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for reading.\n";
        return false;
    }
    
    std::string line;
    std::getline(file, line); // Skip header
    
    int line_num = 1;
    while (std::getline(file, line)) {
        ++line_num;
        if (line.empty()) continue;
        
        try {
            std::vector<std::string> fields = parseCSVLine(line);
            
            if (fields.size() < 18) {
                std::cerr << "Error: Invalid repair request data on line " << line_num << "\n";
                continue;
            }
            
            int id = std::stoi(fields[0]);
            
            // Parse appliance data
            std::string app_type_str = fields[1];
            ApplianceType app_type = stringToApplianceType(app_type_str);
            std::string brand = fields[2];
            std::string model = fields[3];
            int year = std::stoi(fields[4]);
            int catalog_price = std::stoi(fields[5]);
            
            // Create appliance based on type
            std::unique_ptr<Appliance> appliance;
            if (app_type == ApplianceType::Fridge) {
                bool has_freezer = (fields[6] == "1");
                appliance = ApplianceFactory::createApplianceFridge(
                    brand, model, year, catalog_price, has_freezer
                );
            } else if (app_type == ApplianceType::TV) {
                double diag_val = std::stod(fields[7]);
                DiagonalUnit unit = (fields[8] == "cm") ? DiagonalUnit::Centimeters : DiagonalUnit::Inches;
                appliance = ApplianceFactory::createApplianceTV(
                    brand, model, year, catalog_price, diag_val, unit
                );
            } else if (app_type == ApplianceType::WashingMachine) {
                double capacity = 0.0;
                if (!fields[9].empty()) {
                    capacity = std::stod(fields[9]);
                }
                appliance = ApplianceFactory::createApplianceWashingMachine(
                    brand, model, year, catalog_price, capacity
                );
            }
            
            // Parse timestamp
            std::time_t time_t_val = std::stoll(fields[10]);
            auto timestamp = std::chrono::system_clock::from_time_t(time_t_val);
            
            // Parse request data
            int complexity = std::stoi(fields[11]);
            int repair_duration = std::stoi(fields[12]);
            int remaining_time = std::stoi(fields[13]);
            int price = std::stoi(fields[14]);
            
            // Parse status
            std::string status_str = fields[15];
            Status status;
            if (status_str == "Received") status = Status::Received;
            else if (status_str == "Pending") status = Status::Pending;
            else if (status_str == "InProgress") status = Status::InProgress;
            else if (status_str == "Completed") status = Status::Completed;
            else if (status_str == "Rejected") status = Status::Rejected;
            else status = Status::Received; // Default
            
            int receptionist_id = std::stoi(fields[16]);
            int technician_id = std::stoi(fields[17]);
            
            // Create repair request using factory
            auto request = requestFactory.createRepairRequestFromCSV(
                id, std::move(appliance), timestamp, complexity,
                repair_duration, remaining_time, price, status,
                receptionist_id, technician_id
            );
            
            if (request) {
                addRepairRequest(std::move(request));
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Error parsing repair request on line " << line_num << ": " << e.what() << "\n";
        }
    }
    
    file.close();
    return true;
}

// ============================================================================
// PUBLIC SAVE/LOAD FUNCTIONS
// ============================================================================

bool ServiceManager::saveToCSV(const std::string& employees_file, 
                                const std::string& requests_file) {
    bool emp_success = saveEmployeesToCSV(employees_file);
    bool req_success = saveRepairRequestsToCSV(requests_file);
    
    if (emp_success && req_success) {
        std::cout << "Data successfully saved to " << employees_file 
                  << " and " << requests_file << "\n";
        return true;
    }
    return false;
}

bool ServiceManager::loadFromCSV(RepairRequestFactory& requestFactory,
                                  const std::string& employees_file, 
                                  const std::string& requests_file) {
    bool emp_success = loadEmployeesFromCSV(employees_file);
    bool req_success = loadRepairRequestsFromCSV(requestFactory, requests_file);
    
    if (emp_success && req_success) {
        std::cout << "Data successfully loaded from " << employees_file 
                  << " and " << requests_file << "\n";
        return true;
    }
    return false;
}

// Add these implementations to ServiceManager.cpp

#include <fstream>
#include <algorithm>
#include <iomanip>

// ============================================================================
// REPORT GENERATION METHODS
// ============================================================================

bool ServiceManager::generateTop3SalariesReport(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for writing.\n";
        return false;
    }
    
    // Header
    file << "Rank,ID,First Name,Last Name,Type,Salary (RON)\n";
    
    // Calculate salaries for all employees
    std::vector<Employee*> employeeList;
    for (const auto& emp : employees) {
        const_cast<Employee*>(emp.get())->setSalary();
        employeeList.push_back(emp.get());
    }
    
    // Sort by salary (descending), then by last name, then by first name
    std::sort(employeeList.begin(), employeeList.end(), 
        [](const Employee* a, const Employee* b) {
            if (a->getSalary() != b->getSalary()) {
                return a->getSalary() > b->getSalary();
            }
            if (a->getLastName() != b->getLastName()) {
                return a->getLastName() < b->getLastName();
            }
            return a->getFirstName() < b->getFirstName();
        });
    
    // Write top 3
    int count = std::min(3, static_cast<int>(employeeList.size()));
    for (int i = 0; i < count; i++) {
        const Employee* emp = employeeList[i];
        
        // Determine employee type string
        std::string typeStr;
        switch (emp->getType()) {
            case EmployeeType::Technician: typeStr = "Technician"; break;
            case EmployeeType::Receptionist: typeStr = "Receptionist"; break;
            case EmployeeType::Supervisor: typeStr = "Supervisor"; break;
            default: typeStr = "Unknown"; break;
        }
        
        file << (i + 1) << ","
             << emp->getId() << ","
             << escapeCSV(emp->getFirstName()) << ","
             << escapeCSV(emp->getLastName()) << ","
             << typeStr << ","
             << emp->getSalary() << "\n";
    }
    
    file.close();
    std::cout << "Report generated: " << filename << "\n";
    return true;
}

bool ServiceManager::generateLongestRepairReport(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for writing.\n";
        return false;
    }
    
    // Find the technician with the longest repair duration
    Technician* longestTech = nullptr;
    int maxDuration = 0;
    int requestId = 0;
    
    for (const auto& emp : employees) {
        // Check if the employee is a Technician
        Technician* tech = dynamic_cast<Technician*>(emp.get());
        if (tech) {
            const std::vector<int>& activeRequests = tech->getActiveRequests();
            for (int reqId : activeRequests) {
                RepairRequest* req = findRepairRequestById(reqId);
                if (req && req->getRepairDuration() > maxDuration) {
                    maxDuration = req->getRepairDuration();
                    longestTech = tech;
                    requestId = reqId;
                }
            }
        }
    }
    
    // Header
    file << "Technician ID,First Name,Last Name,CNP,City,Skills,Request ID,Repair Duration\n";
    
    if (longestTech) {
        file << longestTech->getId() << ","
             << escapeCSV(longestTech->getFirstName()) << ","
             << escapeCSV(longestTech->getLastName()) << ","
             << longestTech->getCNP() << ","
             << escapeCSV(longestTech->getResidence().getCity()) << ",";
        
        // Skills
        const std::vector<Skill>& skills = longestTech->getSkills();
        std::string skillsStr;
        for (size_t i = 0; i < skills.size(); i++) {
            if (i > 0) skillsStr += "; ";
            skillsStr += applianceTypeToString(skills[i].type) + ":" + skills[i].brand;
        }
        file << "\"" << skillsStr << "\","
             << requestId << ","
             << maxDuration << "\n";
    } else {
        file << "No technicians with active repairs found\n";
    }
    
    file.close();
    std::cout << "Report generated: " << filename << "\n";
    return true;
}

bool ServiceManager::generatePendingRequestsReport(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for writing.\n";
        return false;
    }
    
    // Header
    file << "Appliance Type,Brand,Model,Request IDs,Count\n";
    
    // Collect pending requests grouped by type/brand/model
    struct RequestInfo {
        ApplianceType type;
        std::string brand;
        std::string model;
        std::vector<int> requestIds;
    };
    
    std::vector<RequestInfo> pendingRequests;
    
    for (const auto& req : repair_requests) {
        if (req->getStatus() == Status::Pending) {
            const Appliance& app = req->getAppliance();
            
            // Check if we already have this type/brand/model
            bool found = false;
            for (auto& info : pendingRequests) {
                if (info.type == app.getType() && 
                    info.brand == app.getBrand() && 
                    info.model == app.getModel()) {
                    info.requestIds.push_back(req->getId());
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                RequestInfo info;
                info.type = app.getType();
                info.brand = app.getBrand();
                info.model = app.getModel();
                info.requestIds.push_back(req->getId());
                pendingRequests.push_back(info);
            }
        }
    }
    
    // Sort alphabetically by type, then brand, then model
    std::sort(pendingRequests.begin(), pendingRequests.end(),
        [](const RequestInfo& a, const RequestInfo& b) {
            std::string aType = applianceTypeToString(a.type);
            std::string bType = applianceTypeToString(b.type);
            
            if (aType != bType) return aType < bType;
            if (a.brand != b.brand) return a.brand < b.brand;
            return a.model < b.model;
        });
    
    // Write report
    for (const auto& info : pendingRequests) {
        file << applianceTypeToString(info.type) << ","
             << escapeCSV(info.brand) << ","
             << escapeCSV(info.model) << ",";
        
        // Write request IDs (semicolon-separated)
        std::string idsStr;
        for (size_t i = 0; i < info.requestIds.size(); i++) {
            if (i > 0) idsStr += "; ";
            idsStr += std::to_string(info.requestIds[i]);
        }
        file << "\"" << idsStr << "\","
             << info.requestIds.size() << "\n";
    }
    
    file.close();
    std::cout << "Report generated: " << filename << "\n";
    return true;
}