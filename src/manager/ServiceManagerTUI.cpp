// ServiceManagerTUI.cpp
#include "ServiceManagerTUI.hpp"
#include "EmployeeFactory.hpp"
#include "ApplianceFactory.hpp"
#include "AddressFactory.hpp"
#include <sstream>
#include <iomanip>
#include <ctime>

ServiceManagerTUI::ServiceManagerTUI(ServiceManager& mgr, RepairRequestFactory& reqFactory)
    : manager(mgr), requestFactory(reqFactory), currentMenu(MenuState::Main), 
      selectedItem(0), scrollOffset(0) {}

ServiceManagerTUI::~ServiceManagerTUI() {
    if (running) {
        running = false;
        if (tickThread.joinable()) {
            tickThread.join();
        }
    }
}

void ServiceManagerTUI::start() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_RED, COLOR_BLACK);
        init_pair(5, COLOR_WHITE, COLOR_BLUE);
    }
    
    getmaxyx(stdscr, maxY, maxX);
    
    // Start tick thread
    tickThread = std::thread(&ServiceManagerTUI::tickLoop, this);
    
    // Main UI loop
    mainUILoop();
    
    // Cleanup
    running = false;
    if (tickThread.joinable()) {
        tickThread.join();
    }
    endwin();
}

void ServiceManagerTUI::tickLoop() {
    using namespace std::chrono;
    auto nextRun = steady_clock::now();
    
    while (running) {
        nextRun += seconds(1);
        
        {
            std::lock_guard<std::mutex> lock(managerMutex);
            manager.tick();
        }
        
        std::this_thread::sleep_until(nextRun);
    }
}

void ServiceManagerTUI::mainUILoop() {
    while (running) {
        drawUI();
        handleInput();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void ServiceManagerTUI::drawUI() {
    clear();
    drawHeader();
    
    switch (currentMenu) {
        case MenuState::Main:
            drawMainMenu();
            break;
        case MenuState::EmployeeManagement:
            drawEmployeeMenu();
            break;
        case MenuState::RequestManagement:
            drawRequestMenu();
            break;
        case MenuState::ViewStatistics:
            drawStatistics();
            break;
        default:
            break;
    }
    
    drawFooter();
    refresh();
}

void ServiceManagerTUI::drawHeader() {
    attron(COLOR_PAIR(5) | A_BOLD);
    for (int i = 0; i < maxX; i++) {
        mvaddch(0, i, ' ');
    }
    mvprintw(0, (maxX - 35) / 2, "SERVICE MANAGER - Real-time System");
    attroff(COLOR_PAIR(5) | A_BOLD);
    
    // Current time
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::string timeStr = std::ctime(&time);
    timeStr.pop_back(); // Remove newline
    
    attron(COLOR_PAIR(1));
    mvprintw(1, 2, "Time: %s", timeStr.c_str());
    attroff(COLOR_PAIR(1));
    
    mvhline(2, 0, ACS_HLINE, maxX);
}

void ServiceManagerTUI::drawFooter() {
    mvhline(maxY - 3, 0, ACS_HLINE, maxX);
    
    attron(A_DIM);
    mvprintw(maxY - 2, 2, "Navigation: UP/DOWN Move | ENTER Select | ESC Back | Q Quit");
    attroff(A_DIM);
}

void ServiceManagerTUI::drawMainMenu() {
    int centerX = maxX / 2;
    int menuWidth = 60;
    int startX = (maxX - menuWidth) / 2;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "+----------------------------------------------------------+");
    mvprintw(5, startX, "|                      MAIN MENU                           |");
    mvprintw(6, startX, "+----------------------------------------------------------+");
    attroff(COLOR_PAIR(2) | A_BOLD);
    
    const char* options[] = {
        "1. Employee Management",
        "2. Repair Request Management",
        "3. View Live Statistics",
        "4. Exit Application"
    };
    
    int optionStart = 8;
    for (int i = 0; i < 4; i++) {
        if (i == selectedItem) {
            attron(A_REVERSE | A_BOLD | COLOR_PAIR(1));
            mvprintw(optionStart + i * 2, startX + 4, "-> %-50s", options[i]);
            attroff(A_REVERSE | A_BOLD | COLOR_PAIR(1));
        } else {
            mvprintw(optionStart + i * 2, startX + 4, "   %-50s", options[i]);
        }
    }
    
    // Quick stats preview - larger box
    std::lock_guard<std::mutex> lock(managerMutex);
    
    int statsY = optionStart + 10;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(statsY, startX, "+----------------------------------------------------------+");
    mvprintw(statsY + 1, startX, "|                   QUICK OVERVIEW                         |");
    mvprintw(statsY + 2, startX, "+----------------------------------------------------------+");
    attroff(COLOR_PAIR(2) | A_BOLD);
    
    auto received = manager.filter(Status::Received);
    auto pending = manager.filter(Status::Pending);
    auto in_progress = manager.filter(Status::InProgress);
    auto completed = manager.filter(Status::Completed);
    auto rejected = manager.filter(Status::Rejected);
    
    mvprintw(statsY + 3, startX, "|");
    mvprintw(statsY + 3, startX + 59, "|");
    attron(COLOR_PAIR(3));
    mvprintw(statsY + 3, startX + 4, "Received Requests:        %5zu", received.size());
    attroff(COLOR_PAIR(3));
    
    mvprintw(statsY + 4, startX, "|");
    mvprintw(statsY + 4, startX + 59, "|");
    attron(COLOR_PAIR(3));
    mvprintw(statsY + 4, startX + 4, "Pending Requests:         %5zu", pending.size());
    attroff(COLOR_PAIR(3));
    
    mvprintw(statsY + 5, startX, "|");
    mvprintw(statsY + 5, startX + 59, "|");
    attron(COLOR_PAIR(1));
    mvprintw(statsY + 5, startX + 4, "In Progress:              %5zu", in_progress.size());
    attroff(COLOR_PAIR(1));
    
    mvprintw(statsY + 6, startX, "|");
    mvprintw(statsY + 6, startX + 59, "|");
    attron(COLOR_PAIR(2));
    mvprintw(statsY + 6, startX + 4, "Completed:                %5zu", completed.size());
    attroff(COLOR_PAIR(2));
    
    mvprintw(statsY + 7, startX, "|");
    mvprintw(statsY + 7, startX + 59, "|");
    attron(COLOR_PAIR(4));
    mvprintw(statsY + 7, startX + 4, "Rejected:                 %5zu", rejected.size());
    attroff(COLOR_PAIR(4));
    
    mvprintw(statsY + 8, startX, "+----------------------------------------------------------+");
}

void ServiceManagerTUI::drawEmployeeMenu() {
    int menuWidth = 60;
    int startX = (maxX - menuWidth) / 2;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "+----------------------------------------------------------+");
    mvprintw(5, startX, "|              EMPLOYEE MANAGEMENT                         |");
    mvprintw(6, startX, "+----------------------------------------------------------+");
    attroff(COLOR_PAIR(2) | A_BOLD);
    
    const char* options[] = {
        "1. Add Technician",
        "2. Add Receptionist",
        "3. Add Supervisor",
        "4. Remove Employee by ID",
        "5. List All Employees",
        "6. Back to Main Menu"
    };
    
    int optionStart = 8;
    for (int i = 0; i < 6; i++) {
        if (i == selectedItem) {
            attron(A_REVERSE | A_BOLD | COLOR_PAIR(1));
            mvprintw(optionStart + i * 2, startX + 4, "-> %-50s", options[i]);
            attroff(A_REVERSE | A_BOLD | COLOR_PAIR(1));
        } else {
            mvprintw(optionStart + i * 2, startX + 4, "   %-50s", options[i]);
        }
    }
}

void ServiceManagerTUI::drawRequestMenu() {
    int menuWidth = 60;
    int startX = (maxX - menuWidth) / 2;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "+----------------------------------------------------------+");
    mvprintw(5, startX, "|         REPAIR REQUEST MANAGEMENT                        |");
    mvprintw(6, startX, "+----------------------------------------------------------+");
    attroff(COLOR_PAIR(2) | A_BOLD);
    
    const char* options[] = {
        "1. Add New Repair Request",
        "2. View Request by ID",
        "3. List All Requests",
        "4. Filter Requests by Status",
        "5. Back to Main Menu"
    };
    
    int optionStart = 8;
    for (int i = 0; i < 5; i++) {
        if (i == selectedItem) {
            attron(A_REVERSE | A_BOLD | COLOR_PAIR(1));
            mvprintw(optionStart + i * 2, startX + 4, "-> %-50s", options[i]);
            attroff(A_REVERSE | A_BOLD | COLOR_PAIR(1));
        } else {
            mvprintw(optionStart + i * 2, startX + 4, "   %-50s", options[i]);
        }
    }
}

void ServiceManagerTUI::drawStatistics() {
    std::lock_guard<std::mutex> lock(managerMutex);
    
    int boxWidth = 70;
    int startX = (maxX - boxWidth) / 2;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "+----------------------------------------------------------------------+");
    mvprintw(5, startX, "|                      LIVE STATISTICS                                 |");
    mvprintw(6, startX, "+----------------------------------------------------------------------+");
    attroff(COLOR_PAIR(2) | A_BOLD);
    
    auto received = manager.filter(Status::Received);
    auto pending = manager.filter(Status::Pending);
    auto in_progress = manager.filter(Status::InProgress);
    auto completed = manager.filter(Status::Completed);
    auto rejected = manager.filter(Status::Rejected);
    
    int row = 9;
    
    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(row++, startX + 2, "Status Breakdown:");
    attroff(COLOR_PAIR(3) | A_BOLD);
    row++;
    
    mvprintw(row++, startX + 4, "Received:      %4zu requests", received.size());
    mvprintw(row++, startX + 4, "Pending:       %4zu requests", pending.size());
    mvprintw(row++, startX + 4, "In Progress:   %4zu requests", in_progress.size());
    mvprintw(row++, startX + 4, "Completed:     %4zu requests", completed.size());
    mvprintw(row++, startX + 4, "Rejected:      %4zu requests", rejected.size());
    
    row += 2;
    attron(COLOR_PAIR(2) | A_BOLD);
    size_t total = received.size() + pending.size() + in_progress.size() + 
                   completed.size() + rejected.size();
    mvprintw(row++, startX + 2, "Total Requests: %zu", total);
    attroff(COLOR_PAIR(2) | A_BOLD);
    
    mvprintw(maxY - 5, startX + 2, "Press ESC to return to main menu");
}

void ServiceManagerTUI::handleInput() {
    int ch = getch();
    
    if (ch == 'q' || ch == 'Q') {
        running = false;
        return;
    }
    
    if (ch == 27) { // ESC
        if (currentMenu != MenuState::Main) {
            currentMenu = MenuState::Main;
            selectedItem = 0;
        }
        return;
    }
    
    // Get max items for current menu
    int maxItems = 0;
    switch (currentMenu) {
        case MenuState::Main: maxItems = 4; break;
        case MenuState::EmployeeManagement: maxItems = 6; break;
        case MenuState::RequestManagement: maxItems = 5; break;
        default: maxItems = 1; break;
    }
    
    switch (ch) {
        case KEY_UP:
            if (selectedItem > 0) selectedItem--;
            break;
            
        case KEY_DOWN:
            if (selectedItem < maxItems - 1) selectedItem++;
            break;
            
        case 10: // ENTER
        case KEY_ENTER:
            handleSelection();
            break;
    }
}

void ServiceManagerTUI::handleSelection() {
    if (currentMenu == MenuState::Main) {
        switch (selectedItem) {
            case 0:
                currentMenu = MenuState::EmployeeManagement;
                selectedItem = 0;
                break;
            case 1:
                currentMenu = MenuState::RequestManagement;
                selectedItem = 0;
                break;
            case 2:
                currentMenu = MenuState::ViewStatistics;
                selectedItem = 0;
                break;
            case 3:
                running = false;
                break;
        }
    }
    else if (currentMenu == MenuState::EmployeeManagement) {
        switch (selectedItem) {
            case 0: showAddTechnicianForm(); break;
            case 1: showAddReceptionistForm(); break;
            case 2: showAddSupervisorForm(); break;
            case 3: showRemoveEmployeeDialog(); break;
            case 4: showListEmployees(); break;
            case 5:
                currentMenu = MenuState::Main;
                selectedItem = 0;
                break;
        }
    }
    else if (currentMenu == MenuState::RequestManagement) {
        switch (selectedItem) {
            case 0: showAddRequestForm(); break;
            case 1: showViewRequestDialog(); break;
            case 2: showListRequests(); break;
            case 3: showFilterRequestsDialog(); break;
            case 4:
                currentMenu = MenuState::Main;
                selectedItem = 0;
                break;
        }
    }
}

// Employee Management Actions

void ServiceManagerTUI::showAddTechnicianForm() {
    clearContent();
    nodelay(stdscr, FALSE);
    echo();
    curs_set(1);
    
    int startX = 4;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "ADD NEW TECHNICIAN");
    attroff(COLOR_PAIR(2) | A_BOLD);
    mvhline(5, startX, ACS_HLINE, 50);
    
    int row = 7;
    
    std::string firstName = getInput("First Name: ", row++, startX);
    std::string lastName = getInput("Last Name: ", row++, startX);
    std::string cnp = getInput("CNP: ", row++, startX);
    
    mvprintw(row++, startX, "Date of Hiring (YYYY-MM-DD): ");
    char dateStr[20];
    getnstr(dateStr, 19);
    
    // Parse date
    int year, month, day;
    if (sscanf(dateStr, "%d-%d-%d", &year, &month, &day) != 3) {
        showMessage("Invalid date format!");
        noecho();
        curs_set(0);
        nodelay(stdscr, TRUE);
        return;
    }
    
    std::chrono::year_month_day hireDate = 
        std::chrono::year{year} / std::chrono::month{(unsigned)month} / std::chrono::day{(unsigned)day};
    
    row++;
    mvprintw(row++, startX, "RESIDENCE ADDRESS:");
    Address residence = getAddressInput(row, startX);
    
    int techId;
    {
        std::lock_guard<std::mutex> lock(managerMutex);
        auto technician = EmployeeFactory::createTechnician(
            firstName, lastName, cnp, hireDate, std::move(residence)
        );
        
        techId = technician->getId();
        
        // Add skills
        clearContent();
        mvprintw(4, startX, "Add skills to Technician #%d", techId);
        int numSkills = getIntInput("How many skills to add? ", 6, startX);
        
        Technician* tech = dynamic_cast<Technician*>(technician.get());
        
        for (int i = 0; i < numSkills; i++) {
            clearContent();
            mvprintw(4, startX, "Skill %d/%d", i + 1, numSkills);
            
            mvprintw(6, startX, "Appliance Type (Fridge/TV/WashingMachine): ");
            char typeStr[50];
            getnstr(typeStr, 49);
            
            try {
                ApplianceType type = stringToApplianceType(typeStr);
                std::string brand = getInput("Brand: ", 7, startX);
                
                if (tech) {
                    tech->addSkill(type, brand);
                }
            } catch (...) {
                showMessage("Invalid appliance type!");
            }
        }
        
        manager.addEmployee(std::move(technician));
    }
    
    showMessage("Technician added successfully!");
    
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
}

void ServiceManagerTUI::showAddReceptionistForm() {
    clearContent();
    nodelay(stdscr, FALSE);
    echo();
    curs_set(1);
    
    int startX = 4;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "ADD NEW RECEPTIONIST");
    attroff(COLOR_PAIR(2) | A_BOLD);
    mvhline(5, startX, ACS_HLINE, 50);
    
    int row = 7;
    
    std::string firstName = getInput("First Name: ", row++, startX);
    std::string lastName = getInput("Last Name: ", row++, startX);
    std::string cnp = getInput("CNP: ", row++, startX);
    
    mvprintw(row++, startX, "Date of Hiring (YYYY-MM-DD): ");
    char dateStr[20];
    getnstr(dateStr, 19);
    
    int year, month, day;
    if (sscanf(dateStr, "%d-%d-%d", &year, &month, &day) != 3) {
        showMessage("Invalid date format!");
        noecho();
        curs_set(0);
        nodelay(stdscr, TRUE);
        return;
    }
    
    std::chrono::year_month_day hireDate = 
        std::chrono::year{year} / std::chrono::month{(unsigned)month} / std::chrono::day{(unsigned)day};
    
    row++;
    mvprintw(row++, startX, "RESIDENCE ADDRESS:");
    Address residence = getAddressInput(row, startX);
    
    {
        std::lock_guard<std::mutex> lock(managerMutex);
        auto receptionist = EmployeeFactory::createReceptionist(
            firstName, lastName, cnp, hireDate, std::move(residence)
        );
        manager.addEmployee(std::move(receptionist));
    }
    
    showMessage("Receptionist added successfully!");
    
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
}

void ServiceManagerTUI::showAddSupervisorForm() {
    clearContent();
    nodelay(stdscr, FALSE);
    echo();
    curs_set(1);
    
    int startX = 4;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "ADD NEW SUPERVISOR");
    attroff(COLOR_PAIR(2) | A_BOLD);
    mvhline(5, startX, ACS_HLINE, 50);
    
    int row = 7;
    
    std::string firstName = getInput("First Name: ", row++, startX);
    std::string lastName = getInput("Last Name: ", row++, startX);
    std::string cnp = getInput("CNP: ", row++, startX);
    
    mvprintw(row++, startX, "Date of Hiring (YYYY-MM-DD): ");
    char dateStr[20];
    getnstr(dateStr, 19);
    
    int year, month, day;
    if (sscanf(dateStr, "%d-%d-%d", &year, &month, &day) != 3) {
        showMessage("Invalid date format!");
        noecho();
        curs_set(0);
        nodelay(stdscr, TRUE);
        return;
    }
    
    std::chrono::year_month_day hireDate = 
        std::chrono::year{year} / std::chrono::month{(unsigned)month} / std::chrono::day{(unsigned)day};
    
    row++;
    mvprintw(row++, startX, "RESIDENCE ADDRESS:");
    Address residence = getAddressInput(row, startX);
    
    {
        std::lock_guard<std::mutex> lock(managerMutex);
        auto supervisor = EmployeeFactory::createSupervisor(
            firstName, lastName, cnp, hireDate, std::move(residence)
        );
        manager.addEmployee(std::move(supervisor));
    }
    
    showMessage("Supervisor added successfully!");
    
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
}

void ServiceManagerTUI::showRemoveEmployeeDialog() {
    clearContent();
    nodelay(stdscr, FALSE);
    echo();
    curs_set(1);
    
    int startX = 4;
    
    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(4, startX, "REMOVE EMPLOYEE");
    attroff(COLOR_PAIR(4) | A_BOLD);
    mvhline(5, startX, ACS_HLINE, 50);
    
    int id = getIntInput("Enter Employee ID to remove: ", 7, startX);
    
    {
        std::lock_guard<std::mutex> lock(managerMutex);
        manager.removeEmployeeById(id);
    }
    
    showMessage("Employee removed successfully!");
    
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
}

void ServiceManagerTUI::showListEmployees() {
    clearContent();
    nodelay(stdscr, FALSE);
    
    int startX = 4;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "EMPLOYEE LIST");
    attroff(COLOR_PAIR(2) | A_BOLD);
    mvhline(5, startX, ACS_HLINE, 70);
    
    std::lock_guard<std::mutex> lock(managerMutex);
    
    mvprintw(7, startX, "Note: Full employee listing requires access to employee vector.");
    mvprintw(8, startX, "Add a getter method to ServiceManager to list employees.");
    
    waitForKey();
    nodelay(stdscr, TRUE);
}

// Request Management Actions

void ServiceManagerTUI::showAddRequestForm() {
    clearContent();
    nodelay(stdscr, FALSE);
    echo();
    curs_set(1);
    
    int startX = 4;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "ADD NEW REPAIR REQUEST");
    attroff(COLOR_PAIR(2) | A_BOLD);
    mvhline(5, startX, ACS_HLINE, 50);
    
    int row = 7;
    
    mvprintw(row++, startX, "APPLIANCE INFORMATION:");
    auto appliance = getApplianceInput(row, startX);
    row += 12; // Space used by appliance input
    
    int complexity = getIntInput("Complexity (1-10): ", row++, startX);
    int duration = getIntInput("Estimated Duration (seconds): ", row++, startX);
    int receptionistId = getIntInput("Receptionist ID: ", row++, startX);
    
    {
        std::lock_guard<std::mutex> lock(managerMutex);
        auto request = requestFactory.createRepairRequest(
            std::move(appliance), complexity, duration, receptionistId
        );
        manager.addRepairRequest(std::move(request));
    }
    
    showMessage("Repair request added successfully!");
    
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
}

void ServiceManagerTUI::showViewRequestDialog() {
    clearContent();
    nodelay(stdscr, FALSE);
    echo();
    curs_set(1);
    
    int startX = 4;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "VIEW REPAIR REQUEST");
    attroff(COLOR_PAIR(2) | A_BOLD);
    mvhline(5, startX, ACS_HLINE, 50);
    
    int id = getIntInput("Enter Request ID: ", 7, startX);
    
    {
        std::lock_guard<std::mutex> lock(managerMutex);
        RepairRequest* req = manager.findRepairRequestById(id);
        
        if (req) {
            int row = 10;
            attron(COLOR_PAIR(2) | A_BOLD);
            mvprintw(row++, startX, "Request #%d Details:", id);
            attroff(COLOR_PAIR(2) | A_BOLD);
            row++;
            
            mvprintw(row++, startX + 2, "Status:          %s", statusToString(req->getStatus()).c_str());
            mvprintw(row++, startX + 2, "Price:           %d", req->getPrice());
            mvprintw(row++, startX + 2, "Technician ID:   %d", req->getTechnicianId());
            mvprintw(row++, startX + 2, "Receptionist ID: %d", req->getReceptionistId());
            
            const Appliance& app = req->getAppliance();
            mvprintw(row++, startX + 2, "Appliance:       %s %s %s", 
                     applianceTypeToString(app.getType()).c_str(),
                     app.getBrand().c_str(),
                     app.getModel().c_str());
        } else {
            attron(COLOR_PAIR(4) | A_BOLD);
            mvprintw(10, startX, "Request not found!");
            attroff(COLOR_PAIR(4) | A_BOLD);
        }
    }
    
    waitForKey();
    
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
}

void ServiceManagerTUI::showListRequests() {
    clearContent();
    nodelay(stdscr, FALSE);
    
    std::lock_guard<std::mutex> lock(managerMutex);
    
    int startX = 4;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "ALL REPAIR REQUESTS");
    attroff(COLOR_PAIR(2) | A_BOLD);
    mvhline(5, startX, ACS_HLINE, 70);
    
    int row = 7;
    
    auto statuses = {Status::Received, Status::Pending, Status::InProgress, 
                     Status::Completed, Status::Rejected};
    
    for (auto status : statuses) {
        auto requests = manager.filter(status);
        
        if (!requests.empty()) {
            attron(A_BOLD | COLOR_PAIR(1));
            mvprintw(row++, startX, "%s (%zu):", statusToString(status).c_str(), requests.size());
            attroff(A_BOLD | COLOR_PAIR(1));
            
            for (int id : requests) {
                mvprintw(row++, startX + 2, "Request #%d", id);
                if (row > maxY - 6) {
                    mvprintw(row++, startX + 2, "... (more requests)");
                    break;
                }
            }
            row++;
        }
    }
    
    waitForKey();
    nodelay(stdscr, TRUE);
}

void ServiceManagerTUI::showFilterRequestsDialog() {
    clearContent();
    nodelay(stdscr, FALSE);
    echo();
    curs_set(1);
    
    int startX = 4;
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "FILTER REQUESTS BY STATUS");
    attroff(COLOR_PAIR(2) | A_BOLD);
    mvhline(5, startX, ACS_HLINE, 50);
    
    mvprintw(7, startX, "Status options:");
    mvprintw(8, startX + 2, "0 - Received");
    mvprintw(9, startX + 2, "1 - Pending");
    mvprintw(10, startX + 2, "2 - InProgress");
    mvprintw(11, startX + 2, "3 - Completed");
    mvprintw(12, startX + 2, "4 - Rejected");
    
    int statusInt = getIntInput("Enter status number: ", 14, startX);
    
    if (statusInt < 0 || statusInt > 4) {
        showMessage("Invalid status number!");
        noecho();
        curs_set(0);
        nodelay(stdscr, TRUE);
        return;
    }
    
    Status status = static_cast<Status>(statusInt);
    
    clearContent();
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, startX, "FILTERED RESULTS - %s", statusToString(status).c_str());
    attroff(COLOR_PAIR(2) | A_BOLD);
    mvhline(5, startX, ACS_HLINE, 50);
    
    {
        std::lock_guard<std::mutex> lock(managerMutex);
        auto requests = manager.filter(status);
        
        int row = 7;
        mvprintw(row++, startX, "Found %zu requests:", requests.size());
        row++;
        
        for (int id : requests) {
            mvprintw(row++, startX + 2, "Request #%d", id);
            if (row > maxY - 6) break;
        }
    }
    
    waitForKey();
    
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
}

// Helper Methods

void ServiceManagerTUI::showMessage(const std::string& message) {
    nodelay(stdscr, FALSE);
    
    clearContent();
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(maxY / 2, (maxX - message.length()) / 2, "%s", message.c_str());
    attroff(COLOR_PAIR(2) | A_BOLD);
    mvprintw(maxY / 2 + 2, (maxX - 25) / 2, "Press any key to continue...");
    refresh();
    
    getch();
    nodelay(stdscr, TRUE);
}

std::string ServiceManagerTUI::getInput(const std::string& prompt, int row, int col) {
    mvprintw(row, col, "%s", prompt.c_str());
    char input[256];
    getnstr(input, 255);
    return std::string(input);
}

int ServiceManagerTUI::getIntInput(const std::string& prompt, int row, int col) {
    std::string input = getInput(prompt, row, col);
    return std::atoi(input.c_str());
}

void ServiceManagerTUI::waitForKey() {
    mvprintw(maxY - 5, 4, "Press any key to continue...");
    refresh();
    getch();
}

void ServiceManagerTUI::clearContent() {
    for (int i = 3; i < maxY - 3; i++) {
        move(i, 0);
        clrtoeol();
    }
}

std::string ServiceManagerTUI::statusToString(Status status) {
    switch (status) {
        case Status::Received: return "Received";
        case Status::Pending: return "Pending";
        case Status::InProgress: return "In Progress";
        case Status::Completed: return "Completed";
        case Status::Rejected: return "Rejected";
        default: return "Unknown";
    }
}

std::string ServiceManagerTUI::employeeTypeToString(EmployeeType type) {
    switch (type) {
        case EmployeeType::Technician: return "Technician";
        case EmployeeType::Receptionist: return "Receptionist";
        case EmployeeType::Supervisor: return "Supervisor";
        default: return "Unknown";
    }
}

Address ServiceManagerTUI::getAddressInput(int startRow, int col) {
    int row = startRow;
    
    std::string country = getInput("Country: ", row++, col);
    std::string county = getInput("County: ", row++, col);
    std::string city = getInput("City: ", row++, col);
    std::string street = getInput("Street: ", row++, col);
    std::string number = getInput("Number: ", row++, col);
    
    mvprintw(row++, col, "Type (House/Apartment): ");
    char typeStr[20];
    getnstr(typeStr, 19);
    
    if (std::string(typeStr) == "Apartment") {
        std::string building = getInput("Building: ", row++, col);
        std::string floor = getInput("Floor: ", row++, col);
        std::string aptNumber = getInput("Apartment Number: ", row++, col);
        
        return *AddressFactory::createAddressApartment(
            country, county, city, street, number, 
            building, floor, aptNumber
        );
    } else {
        return *AddressFactory::createAddressHouse(
            country, county, city, street, number
        );
    }
}

std::unique_ptr<Appliance> ServiceManagerTUI::getApplianceInput(int startRow, int col) {
    int row = startRow;
    
    mvprintw(row++, col, "Type (Fridge/TV/WashingMachine): ");
    char typeStr[50];
    getnstr(typeStr, 49);
    
    std::string brand = getInput("Brand: ", row++, col);
    std::string model = getInput("Model: ", row++, col);
    int year = getIntInput("Year of Manufacture: ", row++, col);
    int price = getIntInput("Catalog Price: ", row++, col);
    
    std::string type(typeStr);
    
    if (type == "Fridge") {
        mvprintw(row++, col, "Has Freezer? (1=Yes, 0=No): ");
        char freezerStr[5];
        getnstr(freezerStr, 4);
        bool hasFreezer = (std::string(freezerStr) == "1");
        
        return ApplianceFactory::createApplianceFridge(brand, model, year, price, hasFreezer);
    }
    else if (type == "TV") {
        double diagonal = 0.0;
        mvprintw(row++, col, "Diagonal Value: ");
        char diagStr[20];
        getnstr(diagStr, 19);
        diagonal = std::atof(diagStr);
        
        mvprintw(row++, col, "Unit (Centimeters/Inches): ");
        char unitStr[20];
        getnstr(unitStr, 19);
        DiagonalUnit unit = (std::string(unitStr) == "Inches") ? 
                            DiagonalUnit::Inches : DiagonalUnit::Centimeters;
        
        return ApplianceFactory::createApplianceTV(brand, model, year, price, diagonal, unit);
    }
    else if (type == "WashingMachine") {
        double capacity = 0.0;
        mvprintw(row++, col, "Capacity (kg): ");
        char capStr[20];
        getnstr(capStr, 19);
        capacity = std::atof(capStr);
        
        return ApplianceFactory::createApplianceWashingMachine(brand, model, year, price, capacity);
    }
    
    // Default to fridge if unknown
    return ApplianceFactory::createApplianceFridge(brand, model, year, price, false);
}