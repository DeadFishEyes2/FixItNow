#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

class CNPValidator {
public:
    static bool isValid(const std::string& cnp);
    static std::string getValidationError(const std::string& cnp);
    
private:
    static bool hasValidLength(const std::string& cnp);
    static bool hasOnlyDigits(const std::string& cnp);
    static bool hasValidSex(const std::string& cnp);
    static bool hasValidYear(const std::string& cnp);
    static bool hasValidMonth(const std::string& cnp);
    static bool hasValidDay(const std::string& cnp);
    static bool hasValidCounty(const std::string& cnp);
    static bool hasValidChecksum(const std::string& cnp);
    
    static int extractYear(const std::string& cnp);
    static int extractMonth(const std::string& cnp);
    static int extractDay(const std::string& cnp);
    
    static bool isLeapYear(int year);
    static int getDaysInMonth(int month, int year);
};