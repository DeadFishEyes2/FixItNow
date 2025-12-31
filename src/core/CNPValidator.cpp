#include "CNPValidator.hpp"

bool CNPValidator::isValid(const std::string& cnp) {
    return hasValidLength(cnp) &&
           hasOnlyDigits(cnp) &&
           hasValidSex(cnp) &&
           hasValidYear(cnp) &&
           hasValidMonth(cnp) &&
           hasValidDay(cnp) &&
           hasValidCounty(cnp) &&
           hasValidChecksum(cnp);
}

std::string CNPValidator::getValidationError(const std::string& cnp) {
    if (!hasValidLength(cnp)) return "CNP must have exactly 13 digits";
    if (!hasOnlyDigits(cnp)) return "CNP must contain only digits";
    if (!hasValidSex(cnp)) return "Invalid sex digit (first digit must be 1-9)";
    if (!hasValidYear(cnp)) return "Invalid year in CNP";
    if (!hasValidMonth(cnp)) return "Invalid month (must be 01-12)";
    if (!hasValidDay(cnp)) return "Invalid day for the given month/year";
    if (!hasValidCounty(cnp)) return "Invalid county code (must be 01-52)";
    if (!hasValidChecksum(cnp)) return "Invalid CNP checksum";
    return "Valid CNP";
}

bool CNPValidator::hasValidLength(const std::string& cnp) {
    return cnp.length() == 13;
}

bool CNPValidator::hasOnlyDigits(const std::string& cnp) {
    return std::all_of(cnp.begin(), cnp.end(), ::isdigit);
}

bool CNPValidator::hasValidSex(const std::string& cnp) {
    char sex = cnp[0];
    return sex >= '1' && sex <= '9';
}

bool CNPValidator::hasValidYear(const std::string& cnp) {
    int year = extractYear(cnp);
    return year >= 1800 && year <= 2099;
}

bool CNPValidator::hasValidMonth(const std::string& cnp) {
    int month = extractMonth(cnp);
    return month >= 1 && month <= 12;
}

bool CNPValidator::hasValidDay(const std::string& cnp) {
    int year = extractYear(cnp);
    int month = extractMonth(cnp);
    int day = extractDay(cnp);
    
    if (day < 1) return false;
    return day <= getDaysInMonth(month, year);
}

bool CNPValidator::hasValidCounty(const std::string& cnp) {
    int county = std::stoi(cnp.substr(7, 2));
    return county >= 1 && county <= 52;
}

bool CNPValidator::hasValidChecksum(const std::string& cnp) {
    const std::string weights = "279146358279";
    int sum = 0;
    
    for (int i = 0; i < 12; i++) {
        sum += (cnp[i] - '0') * (weights[i] - '0');
    }
    
    int checksum = sum % 11;
    if (checksum == 10) checksum = 1;
    
    return (cnp[12] - '0') == checksum;
}

int CNPValidator::extractYear(const std::string& cnp) {
    char sex = cnp[0];
    int yy = std::stoi(cnp.substr(1, 2));
    
    // Determine century based on sex digit
    if (sex == '1' || sex == '2') return 1900 + yy;
    if (sex == '3' || sex == '4') return 1800 + yy;
    if (sex == '5' || sex == '6') return 2000 + yy;
    if (sex == '7' || sex == '8') return 2000 + yy; // Residents
    if (sex == '9') return 2000 + yy; // Foreign residents
    
    return 1900 + yy; // Default fallback
}

int CNPValidator::extractMonth(const std::string& cnp) {
    return std::stoi(cnp.substr(3, 2));
}

int CNPValidator::extractDay(const std::string& cnp) {
    return std::stoi(cnp.substr(5, 2));
}

bool CNPValidator::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int CNPValidator::getDaysInMonth(int month, int year) {
    const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month < 1 || month > 12) return 0;
    
    int days = daysInMonth[month - 1];
    if (month == 2 && isLeapYear(year)) days = 29;
    
    return days;
}