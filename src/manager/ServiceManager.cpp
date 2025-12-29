#include "ServiceManager.hpp"

void ServiceManager::addEmployee(std::unique_ptr<Employee> employee) {
    employees.push_back(std::move(employee));    
};

void ServiceManager::addRepairRequest(std::unique_ptr<RepairRequest> repair_request) {
    repair_request->setStatus(Status::Pending);
    repair_requests.push_back(std::move(repair_request));
}

RepairRequest* ServiceManager::findRepairRequestById(int id) {
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

std::vector<int> filter(
        Status status,
        std::chrono::system_clock::time_point start,
        std::chrono::system_clock::time_point end,
        int technician_id,
        int receptionist_id)
{

}